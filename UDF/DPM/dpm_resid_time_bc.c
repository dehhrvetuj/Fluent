#include "udf.h"
#include "dpm.h"

/*
#define PARTICLE_FATE_OUTPUT(_FATE_) \
"Particle-fate-time-zone_id-inj_id-part_id: %d\t%e\t%d\t%d\t\t%" int64_fmt "\n",  _FATE_, P_TIME(p), zone_id, P_INJ_ID(P_INJECTION(p)), p->part_id

*/

typedef enum
{
	TRAP = 0,
	ESCAPE,
	REFLECT
	
} FATE;


static int version = 1;
static int release = 2;


DEFINE_EXECUTE_ON_LOADING(report_version, libname)
{
	Set_User_Memory_Name(0,"PART_RESID_TIME");
	
	Message0("\n---------- Loading %s version %d.%d ---------------\n",libname,version,release);	
}

DEFINE_ON_DEMAND(initialize)
{
	Domain *d; /* declare domain pointer since it is not passed as an  
				 argument to the DEFINE macro  */
	Thread *t;
	cell_t c;

	d = Get_Domain(1);     /* Get the domain using Fluent utility */

	/* if memory for the particle variable titles has not been
	* allocated yet, do it now */
	if (NULLP(user_particle_vars)) Init_User_Particle_Vars();
	/* now set the name and label */
	strcpy(user_particle_vars[0].name,"numOfImpacts");
	strcpy(user_particle_vars[0].label,"Impact Number");
	
	/* Loop over all cell threads in the domain */
	thread_loop_c(t,d)
	{	 
		/* Loop over all cells  */
		begin_c_loop(c,t)
		{ 			
			C_UDMI(c,t,0) = 0.0;
		}
		end_c_loop(c,t)
	}		
	
	Message0("\nUser Defined Memory is now initialized!\n"); 
}

DEFINE_ON_DEMAND(normalize_part_resid_time)
{
	real vol = 0.0;
	
	Domain *d; /* declare domain pointer since it is not passed as an  
				 argument to the DEFINE macro  */
	Thread *t;
	cell_t c;

	d = Get_Domain(1);     /* Get the domain using Fluent utility */
	
	/* Loop over all cell threads in the domain */
	thread_loop_c(t,d)
	{	 
		/* Loop over all cells  */
		begin_c_loop(c,t)
		{ 
			vol = C_VOLUME(c,t);
			vol = pow(vol, 1.0/3.0);
			
			C_UDMI(c,t,0) = C_UDMI(c,t,0) / vol;
		}
		end_c_loop(c,t)
	}

	Message0("\nParticle residence time is now normalized!\n");
}

DEFINE_DPM_SCALAR_UPDATE(Part_resid_time, c, t, initialize, p)
{
	real dt = 0.0;
	
	dt = P_TIME(p) - P_TIME0(p);
  
	if (initialize)
	{
	 /* this is the initialization call, set:
	  * p->user[0] contains the melting index, initialize to 0 
	  * viscosity_0 contains the viscosity at the start of a time step*/
	  
		P_USER_REAL(p,0) = 0.0;

		C_UDMI(c,t,0) = 0.0;	 
	}
	
	if ( dt > C_UDMI(c,t,0) )
	{
		C_UDMI(c,t,0) = dt;
	}	
	
}

DEFINE_DIFFUSIVITY(mean_age_diff, c, t, i)
{
	return C_R(c,t) * 2.88e-05 + C_MU_EFF(c,t) / 0.7;
}

DEFINE_SOURCE(mean_age_source, c, t, dS, eqn)
{
	real source = 0.0;
	
	source = C_R(c,t);
	
	dS[eqn] = 0;
	
	return source;
}

DEFINE_DPM_BC(bc_reflect_udf, p, t, f, f_normal, dim)
{
	real alpha;  /* angle of particle path with face normal */ 
   	
	real vn = 0.0;

	real nor_coeff = 0.01;
	real tan_coeff = 0.01;
	
	real normal[3];
	int i, idim = dim;
	
	real NV_VEC(x);
	
	int zone_id = THREAD_ID(t);
	
	/* count the number of impacts */
	(P_USER_REAL(p,0)) += 1;
	
	/* Message("\nNumber of Impacts: %d\n", (int)((P_USER_REAL(p,0)))); */
	
	if( (P_USER_REAL(p,0))>=100 && P_TIME(p) >= 10 )
	{
		/* Message("Particle fate time inj-id part-id: %d\t%e\t%d\t%" int64_fmt "\n",  REFLECT, P_TIME(p), P_INJ_ID(P_INJECTION(p)), p->part_id); */
		#ifdef PARTICLE_FATE_OUTPUT
		printf(PARTICLE_FATE_OUTPUT(REFLECT)); /* must be printf rather than Message */
		#endif
		
		return PATH_END;
	}
	else
	{
		#if RP_2D 
		/* dim is always 2 in 2D compilation. Need special treatment for 2d
		axisymmetric and swirl flows */
		if (rp_axi_swirl)
		{
			real R = sqrt(P_POS(p)[1]*P_POS(p)[1] +
						P_POS(p)[2]*P_POS(p)[2]);
			if (R > 1.e-20)
			{
			  idim = 3;
			  normal[0] = f_normal[0];
			  normal[1] = (f_normal[1]*P_POS(p)[1])/R;
			  normal[2] = (f_normal[1]*P_POS(p)[2])/R;
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

		if(p->type==DPM_TYPE_INERT)
		{
			alpha = M_PI/2. - acos(MAX(-1.,MIN(1.,NV_DOT(normal,P_VEL(p))/
									  MAX(NV_MAG(P_VEL(p)),DPM_SMALL))));
			if ((NNULLP(t)) && (THREAD_TYPE(t) == THREAD_F_WALL))
			F_CENTROID(x,f,t);

			/* calculate the normal component, rescale its magnitude by 
			 the coefficient of restitution and subtract the change */

			/* Compute normal velocity. */
			for(i=0; i<idim; i++)
			vn += P_VEL(p)[i]*normal[i];

			/* Subtract off normal velocity. */
			for(i=0; i<idim; i++)
			P_VEL(p)[i] -= vn*normal[i];

			/* Apply tangential coefficient of restitution. */
			for(i=0; i<idim; i++)
			P_VEL(p)[i] *= tan_coeff;

			/* Add reflected normal velocity. */
			for(i=0; i<idim; i++)
			P_VEL(p)[i] -= nor_coeff*vn*normal[i];  

			/* Store new velocity in P_VEL0 of particle */
			for(i=0; i<idim; i++)
			P_VEL0(p)[i] = P_VEL(p)[i];

			return PATH_ACTIVE;
		
		}
	}
	
	return PATH_ABORT;
}

DEFINE_DPM_BC(bc_escape_udf, p, t, f, f_normal, dim)
{
	int zone_id = THREAD_ID(t);
	
	/* Message("Particle fate time inj-id part-id: %d\t%e\t%d\t%" int64_fmt "\n",  ESCAPE, P_TIME(p), P_INJ_ID(P_INJECTION(p)), p->part_id); */
	#ifdef PARTICLE_FATE_OUTPUT
	printf(PARTICLE_FATE_OUTPUT(ESCAPE));
	#endif
	
	return PATH_END;
}

DEFINE_DPM_BC(bc_trap_udf, p, t, f, f_normal, dim)
{
	int zone_id = THREAD_ID(t);
	
	/* Message("Particle fate time inj-id part-id: %d\t%e\t%d\t%" int64_fmt "\n",  TRAP, P_TIME(p), P_INJ_ID(P_INJECTION(p)), p->part_id); */
	#ifdef PARTICLE_FATE_OUTPUT
	printf(PARTICLE_FATE_OUTPUT(TRAP));
	#endif 
	
	return PATH_END;
}
