#include "udf.h"
#include "dpm.h"

static int version = 1;
static int release = 2;

static int count = 0;

DEFINE_EXECUTE_ON_LOADING(report_version, libname)
{
    Message0("\nLoading %s version %d.%d\n",libname,version,release);
	
	count = 0;
	
	Set_User_Memory_Name(0,"DPM_CON");
	Set_User_Memory_Name(1,"ResidenceTime");
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
		   C_UDMI(c,t,0) = 0.0;
		   C_UDMI(c,t,1) = 0.0;
		}
		end_c_loop(c,t)
 
   }
      
   Message0("\nUser Defined Memory is now initialized!\n"); 
   
	/*
   strcpy(user_particle_vars[0].name,"melting-index");
   strcpy(user_particle_vars[0].label,"Melting Index"); */

}

DEFINE_DPM_SCALAR_UPDATE(Part_Con, c, t, initialize, p)
{
	real dt = 0.0;
	
	dt = P_TIME(p) - P_TIME0(p);
	
	if (initialize)
	{
		count = 0;
		C_UDMI(c,t,1) = 0.0;
	}
		
	else
	{
		count++;		
	}

	if ( dt > C_UDMI(c,t,1) )
	{
		C_UDMI(c,t,1) = dt;
	}
	
 	Message("\nParticle Time Step %e in cell ID %d with time0 %e at time step %d\n", P_DT(p), c, P_TIME0(p), count); 
	Message("\nParticle Residence Time %e  %e\n", C_UDMI(c,t,1), dt); */	
}
