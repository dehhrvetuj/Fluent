#include "udf.h"
#include "prop.h"


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
