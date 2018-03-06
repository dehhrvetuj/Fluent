#include "udf.h"
#include "prop.h"

/* Age of air calculation */
/* 2017-12-17		*/	

/* DT/Dt = diffusition-term + soruce-term */
/* https://www.computationalfluiddynamics.com.au/tips-tricks-calculating-mean-age-of-air-with-cfd/  */


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
