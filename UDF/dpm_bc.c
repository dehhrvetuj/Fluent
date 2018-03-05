/* Macro:	DEFINE_DPM_BC ( name, p, t, f, f_normal, dim)
 	 
Argument types:	Tracked_Particle *p
 	Thread *t
 	face_t f
 	real f_normal[]
 	int dim
 	 
Function returns:	int 

Return type: typedef enum { PATH_END, PATH_ACTIVE, PATH_BREAK, PATH_ABORT, PATH_STOP #if DPM2 , PATH_NON_LOCAL #endif } Path_Status;

Particles have two fates once they hit the wall:
(1) particle is captured by the wall (trapped, using a return of PATH_ABORT)
(2) particle continues (using a return of PATH_ACTIVE) with reflected velocities.

*/


/* Example 1 - reflect boundary condition for inert particles */
/* not working for a moving wall */

#include "udf.h"

DEFINE_DPM_BC(bc_reflect, p, t, f, f_normal, dim)
{
  real alpha;  /* angle of particle path with face normal */
  real vn=0.;
  real nor_coeff = 1.;
  real tan_coeff = 0.3;
  real normal[3];
  int i, idim = dim;
  real NV_VEC(x);

#if RP_2D 
  /* dim is always 2 in 2D compilation. Need special treatment for 2d
     axisymmetric and swirl flows */
  if (rp_axi_swirl)
    {
      real R = sqrt(p->state.pos[1]*p->state.pos[1] +
                    p->state.pos[2]*p->state.pos[2]);
      if (R > 1.e-20)
        {
          idim = 3;
          normal[0] = f_normal[0];
          normal[1] = (f_normal[1]*p->state.pos[1])/R;
          normal[2] = (f_normal[1]*p->state.pos[2])/R;
        }
      else
        {
          for (i=0; i<idim; i++)
            normal[i] = f_normal[i];
        }
    }
  else
#endif
	  
	  
/* ----- 3D mode --------- */
  for (i=0; i<idim; i++)
	normal[i] = f_normal[i];

  if(p->type==DPM_TYPE_INERT)
    {
      alpha = M_PI/2. - acos(MAX(-1.,MIN(1.,NV_DOT(normal,p->state.V)/
                                  MAX(NV_MAG(p->state.V),DPM_SMALL))));
      if ((NNULLP(t)) && (THREAD_TYPE(t) == THREAD_F_WALL))
        F_CENTROID(x,f,t);

      /* calculate the normal component, rescale its magnitude by 
         the coefficient of restitution and subtract the change */

      /* Compute normal velocity. */
      for(i=0; i<idim; i++)
        vn += p->state.V[i]*normal[i];

      /* Subtract off normal velocity. */
      for(i=0; i<idim; i++)
        p->state.V[i] -= vn*normal[i];

      /* Apply tangential coefficient of restitution. */
      for(i=0; i<idim; i++)
        p->state.V[i] *= tan_coeff;

      /* Add reflected normal velocity. */
      for(i=0; i<idim; i++)
        p->state.V[i] -= nor_coeff*vn*normal[i];  

      /* Store new velocity in state0 of particle */
      for(i=0; i<idim; i++)
        p->state0.V[i] = p->state.V[i];
      
      return PATH_ACTIVE;
    }

  return PATH_ABORT;
}


/* Example 2 - how to use DEFINE_DPM_BC for a wall impingement model. */
/*
 *      Copyright 1988-2002 Fluent Inc and Fluent Deutschland GmbH.
 *      All Rights Reserved
 *
 *      This is unpublished proprietary source code of Fluent Inc.
 *      It is protected by U.S. copyright law as an unpublished work
 *      and is furnished pursuant to a written license agreement.  It
 *      is considered by Fluent Inc. to be confidential and may not be
 *      used, copied, or disclosed to others except in accordance with
 *      the terms and conditions of the license agreement.
 */

#include "udf.h"
#include "dpm.h"
#include "surf.h"
#include "random.h"

/* define a user-defined dpm boundary condition routine
 * bc_reflect: name
 * p:          the tracked particle
 * t:          the touched face thread
 * f:          the touched face 
 * f_normal:   normal vector of touched face
 * dim:        dimension of the problem (2 in 2d and 2d-axi-swirl, 3 in 3d)
 * 
 * return is the status of the particle, see enumeration of Path_Status
 * in dpm.h
 */

#define V_CROSS(a,b,r)\
        ((r)[0] = (a)[1]*(b)[2] - (b)[1]*(a)[2],\
         (r)[1] = (a)[2]*(b)[0] - (b)[2]*(a)[0],\
         (r)[2] = (a)[0]*(b)[1] - (b)[0]*(a)[1])
DEFINE_DPM_BC(bc_wall_jet, p, thread, f, f_normal, dim)
{
  /*
     Routine implementing the Naber and Reitz Wall
     impingement model (SAE 880107)
  */

  real normal[3];
  real tan_1[3];
  real tan_2[3];
  real rel_vel[3];
  real face_vel[3];

  real alpha, beta, phi, cp, sp;
  real rel_dot_n, vmag, vnew, dum;
  real weber_in, weber_out;

  int i, idim = dim;

  boolean moving = (SV_ALLOCATED_P (thread,SV_WALL_GRID_V) &&
		    SV_ALLOCATED_P (thread,SV_WALL_V     )   );

#if RP_2D 
  if (rp_axi_swirl)
    {
      real R = sqrt(p->state.pos[1]*p->state.pos[1] +
		    p->state.pos[2]*p->state.pos[2]);

      if (R > 1.e-20)
	{
	  idim = 3;
	  normal[0] = f_normal[0];
	  normal[1] = (f_normal[1]*p->state.pos[1])/R;
	  normal[2] = (f_normal[1]*p->state.pos[2])/R;
	}
      else
	{
	  for (i=0; i<idim; i++)
	    normal[i] = f_normal[i];
	}
    }
  else
#endif
    for (i=0; i<idim; i++)
      normal[i] = f_normal[i];

  /* 
     Set up velocity vectors and calculate the Weber number
     to determine the regime.
  */

  for (i=0; i < idim; i++)
    {
      if (moving)
	face_vel[i] = WALL_F_VV(f,thread)[i] + WALL_F_GRID_VV(f,thread)[i];
      else
	face_vel[i] = 0.0;

      rel_vel[i] = P_VEL(p)[i] - face_vel[i];
    }
   
  vmag = MAX(NV_MAG(rel_vel),DPM_SMALL);

  rel_dot_n = MAX(NV_DOT(rel_vel,normal),DPM_SMALL);

  weber_in = P_RHO(p) * DPM_SQR(rel_dot_n) * P_DIAM(p) / 
     MAX( DPM_SURFTEN(p), DPM_SMALL );

  /* 
     Regime where bouncing occurs (We_in < 80).
     (Data from Mundo, Sommerfeld and Tropea
      Int. J. of Multiphase Flow, v21, #2, pp151-173, 1995)
  */

  if (weber_in <= 80.)
    {
      weber_out = 0.6785*weber_in*exp(-0.04415*weber_in);
      vnew = rel_dot_n * (1.0 + sqrt( weber_out / 
         MAX( weber_in, DPM_SMALL )));

      /* 
	 The normal component of the velocity is changed based
	 on the experimental paper above (i.e. the Weber number
	 is based on the relative velocity).
      */

      for (i=0; i < idim; i++)
	P_VEL(p)[i] = rel_vel[i] - vnew*normal[i] + face_vel[i];

    }

  if (weber_in > 80.)
    {
      alpha = acos(-rel_dot_n/vmag);

      /* 
         Get one tangent vector by subtracting off the normal
         component from the impingement vector, then cross the 
         normal with the tangent to get an out of plane vector.
      */

      for (i=0; i < idim; i++)
	tan_1[i] = rel_vel[i] - rel_dot_n*normal[i];

      UNIT_VECT(tan_1,tan_1);

      V_CROSS(tan_1,normal,tan_2);

      /*
         beta is calculated by neglecting the coth(alpha) 
         term in the paper (it is approximately right).
      */

      beta = MAX(M_PI*sqrt(sin(alpha)/(1.0-sin(alpha))),DPM_SMALL);

      phi= -M_PI/beta*log(1.0-cheap_uniform_random()*(1.0-exp(-beta)));

      if (cheap_uniform_random() > 0.5) 
	phi = -phi;

      vnew = vmag;

      cp = cos(phi);
      sp = sin(phi);

      for (i=0; i < idim; i++)
	P_VEL(p)[i] = vnew*(tan_1[i]*cp + tan_2[i]*sp) + face_vel[i];

    }

  /*
    Subtract off from the original state.
  */
  for (i=0; i < idim; i++)
    P_VEL0(p)[i] = P_VEL(p)[i];

  if ( DPM_STOCHASTIC_P(p->injection) )
    {

      /* Reflect turbulent fluctuations also */
      /* Compute normal velocity. */

      dum = 0;
      for(i=0; i<idim; i++)
	dum += p->V_prime[i]*normal[i];
      
      /* Subtract off normal velocity. */

      for(i=0; i<idim; i++)
	p->V_prime[i] -= 2.*dum*normal[i];
    }
  return PATH_ACTIVE;
}


/**********************************************************************/
/* Example 3 - UDF Particle Trap Criteria */

/* If a particle impacts the wall and its velocity is less than the 
capture velocity for its diameter it will be trapped. if not it will reflect. */

#include "udf.h"
#include "dpm.h"
DEFINE_DPM_BC(best_dpmbc,p,t,f,f_normal,dim)
{
    /*Variable declarations*/
    real capture_vel;
    real diameter;
    real vmag;  /*particle velocity magnitude*/
    int i;  /*working counter for script*/

    /*Varaible evaluations*/
    diameter=P_DIAM(p);

    /*Calculate capture velocity*/
    capture_vel=(0.000000515/diameter);

    /*Calculate velocity magnitude of particle*/
    for(i=0; i<dim; i++)
    {
        vmag += P_VEL(p)[i]*P_VEL(p)[i];
    }
    vmag = sqrt(vmag);

    /*If particle velocity is less than capture velocity, set to zero*/
    if(vmag < capture_vel)
    {
        for(i=0; i<dim; i++)
            P_VEL(p)[i] = 0;
    }
    /*Stop tracking the particle*/
    return PATH_ABORT;
}
/**********************************************************************/
