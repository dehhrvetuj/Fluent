#include "udf.h"
#include <stdlib.h>
#include <stdio.h>

/* This UDF can be used for parallel run.
NOTE: '%e' should be used instead of '%f' at Line 62 if DPM concentration is to be output. */
 
static real var = 0.0;
static real vol = 0.0;

static const real SIZE[ND_ND] = {0.25,0.25,0.2};
static const real POS[ND_ND] = {0.0,0.0,0.0};

int Weight(const real x[ND_ND], const real pos[ND_ND]);

DEFINE_ON_DEMAND(vol_ave_var)
{
    face_t f;
 	cell_t c;
	Thread *t;
    Domain *domain;
	
	real c_x[ND_ND] = {0.0,0.0,0.0};	

    real c_var = 0.0;
	real c_vol = 0.0;
	
	var = 0.0;
	vol = 0.0;

    domain = Get_Domain(1);

	#if !RP_HOST	/* serial or node; excluding host */
	
	thread_loop_c(t,domain)	/* loop over all cells in the domain */
	{
	   begin_c_loop_int(c,t)	/* looping over interior cells if parallel; loop all cells if serial */
		 {
		   C_CENTROID(c_x,c,t);
		   
		   c_vol = C_VOLUME(c,t);
		   c_var = C_P(c,t);	/*C_DPMS_CONCENTRATION(c,t);*/
		   
		   c_vol *= Weight(c_x,POS);
		   
		   vol += c_vol;
		   var += c_var*c_vol;
		 }
	   end_c_loop_int (c,t)
	}
	#endif
	
    
    #if RP_NODE     /* only parallel compute nodes involved here; perform node synchronized action here */        
        var = PRF_GRSUM1(var);
		vol = PRF_GRSUM1(vol);
    #endif	
	
	if ( vol > 1E-16 )
	{
		var /= vol;
		Message0("Volume Averaged Variable at (%f %f %f) %f\n",pos[0],pos[1],pos[2],var); 
	}
	else
	{
		Message0("\nZero volume - wrong specified locations at %f %f %f\n",pos[0],pos[1],pos[2]);
	}	

}


int Weight(const real x[ND_ND], const real pos[ND_ND])
{
	int flag = 1;
	int dim = 0;
	
	for	(dim=0; dim<ND_ND; dim++)
	{
		if (fabs(x[dim]-pos[dim]) <= SIZE[dim]/2.0)
			flag *= 1;
		else
			flag *= 0;
	}
	
	return  flag;
} 

