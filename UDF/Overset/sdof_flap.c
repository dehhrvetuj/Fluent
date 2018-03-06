#include "udf.h"
#include <stdlib.h>

#define CG_DISTANCE 0.5

DEFINE_CG_MOTION(door,dt,vel,omega,time,dtime)
{
    vel[0] = 0.75;
}

#define C1 1.0E7
#define C2 0.0

static int last_ts = -1;   /*  Global variable.  Time step is never <0 */

DEFINE_ADJUST(first_iter_only, domain)
{
  int curr_ts;
  
  curr_ts = N_TIME;
  
  if (last_ts != curr_ts)
  {
    last_ts = curr_ts;

    /* things to be done only on first iteration of each time step 
       can be put here  */
  }
}

int IsPorous(real x[ND_ND])
{
  if ( x[0]<=0.0 && fabs(x[1])<=0.20 )
  {
	return 1;
  }
  else
  {
	return 0;
  }	
} 

void Porousity(int i, real u, real v, real w, real rho, real mu, real* source, real* dS)
{
	real C_l = 0.0;
	real C_q = 0.0;
	
	real V[3] = {0,0,0};
	V[0] = u;
	V[1] = v;
	V[2] = w;
	
	C_l = C1*mu;
	C_q = C2*rho*sqrt(u*u+v*v+w*w)*0.5;
	
	*source = - (C_l + C_q) * V[i];
	*dS = - (C_l + 2*C_q);
}

DEFINE_SOURCE(xmom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real source;

  C_CENTROID(x,c,t);

  if (IsPorous(x))
  {
	Porousity(0, C_U(c,t), C_V(c,t), C_W(c,t), C_R(c,t), C_MU_L(c,t), &source, &(dS[eqn]));
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;
  }


  return source;
}

DEFINE_SOURCE(ymom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real source;

  C_CENTROID(x,c,t);

  if (IsPorous(x))
  {
	Porousity(1, C_U(c,t), C_V(c,t), C_W(c,t), C_R(c,t), C_MU_L(c,t), &source, &(dS[eqn]));
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;
  }
  
  return source;
}

DEFINE_SOURCE(zmom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real source;

  C_CENTROID(x,c,t);

  if (IsPorous(x))
  {
	Porousity(2, C_U(c,t), C_V(c,t), C_W(c,t), C_R(c,t), C_MU_L(c,t), &source, &(dS[eqn]));
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;
  }


  return source;
}

DEFINE_ON_DEMAND(mark_porous_media)
{
  real x[ND_ND];
  
  Domain *domain;
  cell_t c;
  Thread *t;
 
  domain=Get_Domain(1);	/* In the case of single phase, domain ID is 1. */
  
  
  /* Fill the UDM with magnitude of gradient. */
  thread_loop_c (t,domain)
    {
       begin_c_loop (c,t)
         {
		   C_CENTROID(x,c,t);
           C_UDMI(c,t,0) = IsPorous(x);
         }
       end_c_loop (c,t)
    }
}
