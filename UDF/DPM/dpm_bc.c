#include "udf.h"
#include "dpm.h"

static int version = 1;
static int release = 2;

static int count = 0;

DEFINE_EXECUTE_ON_LOADING(report_version, libname)
{
	Message0("\nLoading %s version %d.%d\n",libname,version,release);
	count = 0;
	/*
	Set_User_Memory_Name(0,"DPM_CON");
	Set_User_Memory_Name(1,"ResidenceTime"); */
}

DEFINE_ON_DEMAND(on_demand_calc)
{
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
		/*
		   C_UDMI(c,t,0) = 0.0;
		   C_UDMI(c,t,1) = 0.0; */
		}
		end_c_loop(c,t)

	}
	  
	Message0("\nUser Defined Memory is now initialized!\n"); 

	strcpy(user_particle_vars[0].name,"melting-index");
	strcpy(user_particle_vars[0].label,"Melting Index");

}

DEFINE_DPM_SCALAR_UPDATE(Part_Con, c, t, initialize, p)
{
	if (initialize)
	{
		 /* this is the initialization call, set:
		  * p->user[0] contains the melting index, initialize to 0 
		  * viscosity_0 contains the viscosity at the start of a time step*/

		 P_USER_REAL(p,0) = 0.; 

	}

	else
	{
		Message("\nParticle Time Step %e in cell ID %d with time0 %e\n", P_DT(p), c, P_TIME0(p)); 
	}	

}

DEFINE_DPM_BC(bc_reflect, p, t, f, f_normal, dim)
{
	real alpha = 0.0;  /* angle of particle path with face normal */ 

	real vn = 0.0;

	real nor_coeff = 0.1;
	real tan_coeff = 0.1;

	real normal[3];
	int i = 0, idim = dim;

	P_USER_REAL(p,0) += 1;

	/* Particle residence time > 10 second or number of impacts > 100 */
	if( P_TIME(p) > 10  || P_USER_REAL(p,0) > 100 )
	{
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
