/*********************************************************************
UDF for computing the melting index along a particle trajectory
**********************************************************************/
#include "udf.h"
DEFINE_INIT(melt_setup,domain)
{
	/* if memory for the particle variable titles has not been
	* allocated yet, do it now */
	if (NULLP(user_particle_vars)) Init_User_Particle_Vars();
	/* now set the name and label */
	strcpy(user_particle_vars[0].name,"melting-index");
	strcpy(user_particle_vars[0].label,"Melting Index");
	strcpy(user_particle_vars[1].name,"melting-index-0");
	strcpy(user_particle_vars[1].label,"Melting Index 0");
}

/* update the user scalar variables */
DEFINE_DPM_SCALAR_UPDATE(melting_index,cell,thread,initialize,p)
{
	cphase_state_t *c = &(p->cphase);
	if (initialize)
	{
		/* this is the initialization call, set:
		* P_USER_REAL(p,0) contains the melting index, initialize to 0
		* P_USER_REAL(p,1) contains the viscosity at the start of a time step*/
		P_USER_REAL(p,0) = 0.;
		P_USER_REAL(p,1) = c->mu;
	}
	else
	{
		/* use a trapezoidal rule to integrate the melting index */
		P_USER_REAL(p,0) += P_DT(p) * .5 * (1/P_USER_REAL(p,1) + 1/c->mu);
		/* save current fluid viscosity for start of next step */
		P_USER_REAL(p,1) = c->mu;
	}
}
/* write melting index when sorting particles at surfaces */
DEFINE_DPM_OUTPUT(melting_output,header,fp,p,thread,plane)
{
	char name[100];
	if (header)
	{
		if (NNULLP(thread))
			par_fprintf_head(fp,"(%s %d)\n",THREAD_HEAD(thread)->
			dpm_summary.sort_file_name,11);
		else
			par_fprintf_head(fp,"(%s %d)\n",plane->sort_file_name,11);
			par_fprintf_head(fp,"(%10s %10s %10s %10s %10s %10s %10s"
			" %10s %10s %10s %10s %s)\n",
			"X","Y","Z","U","V","W","diameter","T","mass-flow",
			"time","melt-index","name");
	}
	else
	{
		sprintf(name,"%s:%d",P_INJECTION(p)->name,p->part_id);
		/* add P_INJ_ID(P_INJECTION(p)) and part_id for sorting in parallel */
		
		par_fprintf(fp,
		"%d %d ((%10.6g %10.6g %10.6g %10.6g %10.6g %10.6g "
		"%10.6g %10.6g %10.6g %10.6g %10.6g) %s)\n",
		P_INJ_ID(P_INJECTION(p)), p->part_id,
		P_POS(p)[0], P_POS(p)[1], P_POS(p)[2],
		P_VEL(p)[0], P_VEL(p)[1], P_VEL(p)[2],
		P_DIAM(p), P_T(p), P_FLOW_RATE(p), P_TIME(p),
		P_USER_REAL(p,0), name);
	}
}
