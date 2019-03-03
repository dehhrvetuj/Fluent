/********************************************************************
	UDF for averaging DPM concentration .
	This UDF can be used for parallel run.  
	NOTE: '%e' should be used instead of '%f' 
	at Line 62 if DPM concentration is to be output. 
*********************************************************************/

#include "udf.h"
#include <stdlib.h>
#include <stdio.h>


#define MAX_CHAR 256
#define MAX_POS 512
#define TEMPERATURE_KELVIN 273.15

static real POS[MAX_POS][ND_ND] = {0}; 
static const real SIZE[ND_ND] = {0.4,0.4,0.4}; 

static real vol = 0.0;
static real con = 0.0;
static real tmp	= 0.0;
static real vel[ND_ND] = {0.0,0.0,0.0};


int Weight(real x[ND_ND], real pos[ND_ND]);


DEFINE_ON_DEMAND(dpm_ave)
{
	real x[ND_ND] = {0.0,0.0,0.0};	
	real pos[ND_ND] = {0.0,0.0,0.0};
	
	real c_vol = 0.0;
	real c_con = 0.0;
	real c_tmp = 0.0;
	real c_vel[ND_ND] = {0.0,0.0,0.0};
	
	real weight = 0.0;	
	
	int i = 0, dim = 0, index = 0;
	
	#if !RP_NODE		/* serial or host */
	FILE * fp = NULL;
	#endif

	#if !RP_HOST 		/* serial or node */
	Domain *domain;
	cell_t c;
	Thread *t;
	#endif
	
	#if !RP_HOST 		/* serial or node */	
	domain = Get_Domain(1);	/* In the case of single phase, domain ID is 1. */
	#endif
	
	#if !RP_NODE		/* serial or host */
	fp = fopen("positions.txt", "r");		/* open the positions file */
	
	if( fp == NULL )
		Message("\nCannot find the file\n");
	else
	{
		Message("\n ------------- File positions.txt is open. --------------\n");
		
		if ( index >= MAX_POS )
			Message("\n ----------Too many specified locations.--------------\n");
		else
			while( fscanf(fp, "%lf %lf %lf", &POS[index][0],&POS[index][1],&POS[index][2]) == 3 )
			{
				index++;
			}
		
		Message("\n ---------- positions.txt file closed --------------\n");
	}
	
	fclose(fp); /* close the position file */
	#endif

	
	#if !RP_NODE		/* serial or host */
	fp = NULL;
	fp = fopen("output.txt", "w");		/* open the output file */

	if( fp == NULL )
		Message("\n-------------Cannot write to file output.txt-----------------\n");
	else
		Message("\n--------------Open file output.txt-----------------\n");

	fprintf(fp, "%s\n", "x y z T c");
	#endif

	host_to_node_int_1(index);
	
	for(i=0; i<index; i++)	/* start looping all position points */
	{
		#if !RP_NODE		/* serial or host */		
		NV_V(pos, =, POS[i]);
		#endif
		
		host_to_node_real(pos,3);

		#if !RP_HOST	/* serial or node; excluding host */
		weight = 0.0;
		
		vol = 0.0;
		con = 0.0;
		tmp = 0.0;
		
		ND_SET(vel[0], vel[1], vel[2], 0.0, 0.0, 0.0);
		
		thread_loop_c (t,domain)		/* loop over all cells */
		{
			if (FLUID_THREAD_P(t))
			{
				begin_c_loop (c,t)
				{
					C_CENTROID(x,c,t);

					c_tmp = C_T(c,t);
					c_vol = C_VOLUME(c,t);
					c_con = C_DPMS_CONCENTRATION(c,t);
					ND_SET(c_vel[0], c_vel[1], c_vel[2], C_U(c, t), C_V(c, t), C_W(c, t)); 
				

					weight = Weight(x,pos);
					c_vol *= weight;

					vol += c_vol;
					con += c_con*c_vol;
					tmp += c_tmp*c_vol;
					NV_V_VS(vel, =, vel, +, c_vel, *, c_vol);
				}
				end_c_loop (c,t)
			}
		}
		#endif
	
		#if RP_NODE     /* only parallel compute nodes involved here; perform node synchronized action here */        
		con = PRF_GRSUM1(con);
		tmp = PRF_GRSUM1(tmp);
		vol = PRF_GRSUM1(vol);
		#endif	

		node_to_host_real_1(con);
		node_to_host_real_1(tmp);
		node_to_host_real_1(vol);
		
		#if !RP_NODE		/* serial or host */
		if ( vol > 1E-16 )
		{
			Message("Averaged DPM concentration at %f %f %f is %e\n", 
				pos[0],pos[1],pos[2],con/vol);
			fprintf(fp, "%f %f %f %f %e\n", 
				pos[0],pos[1],pos[2],(tmp/vol-TEMPERATURE_KELVIN),con/vol);
			
		}
		else
		{
			Message("\nZero volume - wrong specified locations at %f %f %f\n",pos[0],pos[1],pos[2]);
			fprintf(fp, "%f %f %f 0 0\n", pos[0],pos[1],pos[2]);
		}
		#endif	

	}

	#if !RP_NODE		/* serial or host */
	fclose(fp);
	Message("\n--------------- Output.txt file closed. -----------------\n");
	#endif

	
}

int Weight(real x[ND_ND], real pos[ND_ND])
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
	
	return flag;
} 