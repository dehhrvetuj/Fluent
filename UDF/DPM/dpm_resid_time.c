#include "udf.h"
#include "dpm.h"

DEFINE_DPM_SCALAR_UPDATE(Part_Con, c, t, initialize, p)
{
	real dt = 0.0;
	
	dt = P_TIME(p) - P_TIME0(p);
	
	if ( dt > C_UDMI(c,t,1) )
	{
		C_UDMI(c,t,1) = dt;
	}
	
}
