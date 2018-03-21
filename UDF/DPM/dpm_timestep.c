/* Time step control UDF for DPM  */

#include "udf.h"
#include "dpm.h"

DEFINE_DPM_TIMESTEP(limit_to_e_minus_four,p dt)
{
   if (dt > 1.e-4)
     {
/*       p->next_time_step = 1.e-4; */
       return 1.e-4;
     }

   return dt;
}




/* Particle time step control UDF for DPM  */

#include "udf.h"
#include "dpm.h"

DEFINE_DPM_TIMESTEP(limit_to_fifth_of_prt,p,dt)
{
   real drag_factor = 0.;
   real p_relax_time;
   cphase_state_t *c = &(p->cphase);

   /* compute particle relaxation time */
   if (P_DIAM(p) != 0.0)
     drag_factor = DragCoeff(p) * c->mu / ( P_RHO(p) * P_DIAM(p) * P_DIAM(p));
   else
     drag_factor = 1.;

   p_relax_time = 1./drag_factor;

   /* check the condition and return the time step */
   if (dt > p_relax_time/5.)
     {
       return p_relax_time/5.;
     }

   return dt;
}
