#include "udf.h"
#include <stdlib.h>
#include <stdio.h>

/* 
NOTE: this UDF is written only for serial use. 
Cannot be used for parallel applications!!!
*/

#if ND_ND==3

#define MAX_CHAR 256
#define MAX_POS 512
#define TEMPERATURE_KELVIN 273.15

/* static char str[MAX_CHAR] = {0}; */
static real POS[MAX_POS][ND_ND] = {0}; 

static const real SIZE[ND_ND] = {0.1,0.1,0.1}; 

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
	
	FILE * fp = NULL;
	
	Domain *domain;
	cell_t c;
	Thread *t;
	
	domain=Get_Domain(1);	/* In the case of single phase, domain ID is 1. */
		
	/* open the positions file */
	fp = fopen("positions.txt", "r");
	
	if( fp == NULL )
	{
		Message("\nCannot find the file\n");
	}
	else
	{
		Message("\n ------------- File positions.txt is open. --------------\n");
		
		if ( index > MAX_POS )
			Message("\n ----------Too many specified locations.--------------\n");
		else
			while( fscanf(fp, "%lf %lf %lf", &POS[index][0],&POS[index][1],&POS[index][2]) == 3 )
			{
				/* while( fgets(str, MAX_CHAR, fp) != NULL ) */
				index++;
			}
	}
	
	fclose(fp); /* close the position file */
	
	/* open the output file */
	fp = fopen("output.txt", "w");
	if( fp == NULL )
		Message("\nCannot write to file output.txt\n");

	/* start looping all position points */
	for(i=0; i<index; i++)
	{
		/* for (dim=0; dim<ND_ND; dim++)
		{
			pos[dim] = POS[i][dim];			
		}*/
		
		NV_V(pos, =, POS[i]);
		
		weight = 0.0;
		
		vol = 0.0;
		con = 0.0;
		tmp = 0.0;
		ND_SET(vel[0], vel[1], vel[2], 0.0, 0.0, 0.0);
		/* vel[0] = 0.0; c_vel[1] = 0.0; c_vel[2] = 0.0; */
		
		/* loop over all cells */
		thread_loop_c (t,domain)
		{
		   begin_c_loop (c,t)
			 {
			   C_CENTROID(x,c,t);
			   
			   c_tmp = C_T(c,t);
			   c_vol = C_VOLUME(c,t);
			   c_con = C_DPMS_CONCENTRATION(c,t);
			   ND_SET(c_vel[0], c_vel[1], c_vel[2], C_U(c, t), C_V(c, t), C_W(c, t)); 
			   /*  c_vel[0] = C_U(c,t); c_vel[1] = C_V(c,t); c_vel[2] = C_W(c,t); */
			   
			   weight = Weight(x,pos);
			   c_vol *= weight;
			   
			   vol += c_vol;
			   con += c_con*c_vol;
			   tmp += c_tmp*c_vol;
			   NV_V_VS(vel, =, vel, +, c_vel, *, c_vol);
			   /* vel[0] += c_vel[0]*c_vol; */
			 }
		   end_c_loop (c,t)
		}
		
		if ( vol > 1E-9 )
		{
			Message("Averaged DPM concentration at %f %f %f is %e\n", 
				pos[0],pos[1],pos[2],con/vol);
			fprintf(fp, "%f %f %f %f %f %f %f %e\n", 
				pos[0],pos[1],pos[2],vel[0]/vol,vel[1]/vol,vel[2]/vol,(tmp/vol-TEMPERATURE_KELVIN),con/vol);
			
		}
		else
		{
			Message("\nZero volume - wrong specified locations at %f %f %f\n",pos[0],pos[1],pos[2]);
			fprintf(fp, "%f %f %f 0 0 0 0 0\n", pos[0],pos[1],pos[2]);
		}	
	}

	fclose(fp);

	Message("\n--------------- User Defined Function DPM_AVE exits. -----------------\n");
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

#else

#endif
