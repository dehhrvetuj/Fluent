/*********************************************************************
  UDF for computing the melting index along a particle trajectory   
**********************************************************************/
#include "udf.h"

static real viscosity_0;

DEFINE_INIT(melt_setup, domain) 
{
        /* if memory for the particle variable titles has not been
         * allocated yet, do it now */

  if (NULLP(user_particle_vars)) Init_User_Particle_Vars();

         /* now set the name and label */

  strcpy(user_particle_vars[0].name,"melting-index");
  strcpy(user_particle_vars[0].label,"Melting Index");
}

        /* update the user scalar variables */
DEFINE_DPM_SCALAR_UPDATE(melting_index, cell, thread, initialize, p)
{
  cphase_state_t *c = &(p->cphase);
  if (initialize)
    {
     /* this is the initialization call, set:
      * p->user[0] contains the melting index, initialize to 0 
      * viscosity_0 contains the viscosity at the start of a time step*/

      p->user[0] = 0.;
      viscosity_0 = c->mu;
    }

  else
    {
      /* use a trapezoidal rule to integrate the melting index */
      p->user[0] += P_DT(p) * .5 * (1/viscosity_0 + 1/c->mu);

      /* save current fluid viscosity for start of next step */
      viscosity_0 = c->mu;
    }
}       

    /* write melting index when sorting particles at surfaces */
DEFINE_DPM_OUTPUT(melting_output, header, fp, p, thread, plane)
{
  char name[100];

  if (header)
   {
   if (NNULLP(thread))
     printf(fp,"(%s %d)\n",thread->head->dpm_summary.sort_file_name,11);
   else
     printf(fp,"(%s %d)\n",plane->sort_file_name,11);
   printf(fp,"(%10s %10s  %10s  %10s  %10s  %10s  %10s"  
               " %10s  %10s  %10s  %10s  %s)\n",
                 "X","Y","Z","U","V","W","diameter","T","mass-flow",
                 "time","melt-index","name");
    }
  else
    {
      sprintf(name,"%s:%d",p->injection->name,p->part_id);
      printf(fp, 
           "((%10.6g  %10.6g  %10.6g  %10.6g  %10.6g  %10.6g  "
           "%10.6g  %10.6g  %10.6g  %10.6g %10.6g) %s)\n",
           p->state.pos[0], p->state.pos[1], p->state.pos[2],
           p->state.V[0], p->state.V[1], p->state.V[2],
           p->state.diam, p->state.temp, p->flow_rate, p->state.time,
           p->user[0], name);
    }
}
