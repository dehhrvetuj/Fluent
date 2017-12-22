#include "udf.h"

/*
 * UDF to compute fluid residence time
 */

/*
 * This UDF requires at least 1 UDS to be defined
 * At inlet, define UDS (value) = 0
 * UDS will have units of time  and represent approximate residence time
 * of fluid in domain
 *
 * Diffusivity of scalar should be small to reduce diffusion i.e. 1E-5
 * For turbulent flow with recirculating regions, it is more appropriate
 * to hook the function mean_age_diff to the UDS diffusivity in the
 * materials panel
 */


DEFINE_SOURCE(rt_source,c,t,dS,eqn)
{
  real source = C_R(c,t);
  dS[eqn] = 0.0;
  return source;
}

/*
 * in the function below, the turbulent schmidt number has been
 * set to 1.0 in accordance with the references mentioned above
 * and the fluid molecular diffusivity has been set to 1.0e-5 m2/s
 *
 * these values can be changed on an as-needed basis, for instance in
 * Airpak mean age of air calculations, a value of 2.88e-5 is used
 * for the molecular diffusivity and a value of 0.7 is used for the
 * turbulent Schmidt number
 *C_R(c,t)*2.88e-05+C_MU_EFF(c,t)/0.7
 */
 
DEFINE_DIFFUSIVITY(rtd_diff, c, t, i)
{
  return C_R(c,t)*1.0e-05+C_MU_EFF(c,t)/1.0;
}