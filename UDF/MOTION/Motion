#include "udf.h"
#include "unsteady.h"
#include "stdio.h" 
#include "stdlib.h"
#define PI 3.1415926
//#define WALKVEL 1.32  //The average walking velocity for a women is about 79m/min
#define WALKVEL 1.085  //walking slowly in hospital ward 2mile/h,2.0met,aver 0.89m/s, max is 1.085, min is 0.11
#define Hipmax 25.81  //degree
#define FREQUENCY 0.74  //as 0.89m/s walking velocity
#define Hipmin 13.09  //degree
#define Starttime 0  //top exhaust, mouth 5, 1um after 750s particle injection


//#define R 0.5 //length of arm in y direction
/************************************************************/
	real current_time = 0.0 ;
	Domain * domain ;
	Thread * thread ;
	real NV_VEC( origin ),NV_VEC( force ),NV_VEC( moment ) ;
/************************************************************/
DEFINE_CG_MOTION(BODY_top_mouth6,dt,vel,omega,time,dtime)
{

	double vv;
	current_time = CURRENT_TIME-Starttime ; 
	

	vel[0] = 0.0; //sin(PI*current_time)
	vel[2] = 0.0 ; 

	if(current_time<=0.15/FREQUENCY)
	vv = WALKVEL ; 
	
	else if(current_time>0.15/FREQUENCY&&current_time<=0.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.15/FREQUENCY);
	
	else if(current_time>0.25/FREQUENCY&&current_time<=0.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.25/FREQUENCY);
	
	else if(current_time>0.35/FREQUENCY&&current_time<=0.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>0.65/FREQUENCY&&current_time<=0.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.65/FREQUENCY);
	
	else if(current_time>0.75/FREQUENCY&&current_time<=0.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.75/FREQUENCY);
	
      
//////first T
	else if(current_time>0.85/FREQUENCY&&current_time<=1.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.15/FREQUENCY&&current_time<=1.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.15/FREQUENCY);
	
	else if(current_time>1.25/FREQUENCY&&current_time<=1.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.25/FREQUENCY);
	
	else if(current_time>1.35/FREQUENCY&&current_time<=1.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.65/FREQUENCY&&current_time<=1.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.65/FREQUENCY);
	
	else if(current_time>1.75/FREQUENCY&&current_time<=1.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.75/FREQUENCY);

////Second T
	else if(current_time>1.85/FREQUENCY&&current_time<=2.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.15/FREQUENCY&&current_time<=2.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.15/FREQUENCY);
	
	else if(current_time>2.25/FREQUENCY&&current_time<=2.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.25/FREQUENCY);
	
	else if(current_time>2.35/FREQUENCY&&current_time<=2.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.65/FREQUENCY&&current_time<=2.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.65/FREQUENCY);
	
	else if(current_time>2.75/FREQUENCY&&current_time<=2.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.75/FREQUENCY);

///third T
	else if(current_time>2.85/FREQUENCY&&current_time<=3.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.15/FREQUENCY&&current_time<=3.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.15/FREQUENCY);
	
	else if(current_time>3.25/FREQUENCY&&current_time<=3.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.25/FREQUENCY);
	
	else if(current_time>3.35/FREQUENCY&&current_time<=3.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.65/FREQUENCY&&current_time<=3.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.65/FREQUENCY);
	
	else if(current_time>3.75/FREQUENCY&&current_time<=3.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.75/FREQUENCY);
///fourth T
	else if(current_time>3.85/FREQUENCY&&current_time<=4.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.15/FREQUENCY&&current_time<=4.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.15/FREQUENCY);
	
	else if(current_time>4.25/FREQUENCY&&current_time<=4.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.25/FREQUENCY);
	
	else if(current_time>4.35/FREQUENCY&&current_time<=4.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.65/FREQUENCY&&current_time<=4.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.65/FREQUENCY);
	
	else if(current_time>4.75/FREQUENCY&&current_time<=4.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.75/FREQUENCY);
	
	vel[1] =vv; 


	omega[0] = 0.0 ;
	omega[1] = 0.0 ;
	omega[2] = 0.0 ;
//	Message("time=%f  omega=%f\n",current_time,vel[1]) ; 
}

/************************************************************/

/************************************************************/
DEFINE_CG_MOTION(LEG1_px_sin3,dt,vel,omega,time,dtime)
{
	double tt;
	double vv;
	current_time = CURRENT_TIME-Starttime ; 
	

	vel[0] = 0.0; //sin(PI*current_time)
	vel[2] = 0.0 ; 

	if(current_time<=0.15/FREQUENCY)
	vv = WALKVEL ; 
	
	else if(current_time>0.15/FREQUENCY&&current_time<=0.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.15/FREQUENCY);
	
	else if(current_time>0.25/FREQUENCY&&current_time<=0.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.25/FREQUENCY);
	
	else if(current_time>0.35/FREQUENCY&&current_time<=0.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>0.65/FREQUENCY&&current_time<=0.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.65/FREQUENCY);
	
	else if(current_time>0.75/FREQUENCY&&current_time<=0.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.75/FREQUENCY);
	
      
//////first T
	else if(current_time>0.85/FREQUENCY&&current_time<=1.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.15/FREQUENCY&&current_time<=1.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.15/FREQUENCY);
	
	else if(current_time>1.25/FREQUENCY&&current_time<=1.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.25/FREQUENCY);
	
	else if(current_time>1.35/FREQUENCY&&current_time<=1.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.65/FREQUENCY&&current_time<=1.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.65/FREQUENCY);
	
	else if(current_time>1.75/FREQUENCY&&current_time<=1.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.75/FREQUENCY);

////Second T
	else if(current_time>1.85/FREQUENCY&&current_time<=2.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.15/FREQUENCY&&current_time<=2.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.15/FREQUENCY);
	
	else if(current_time>2.25/FREQUENCY&&current_time<=2.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.25/FREQUENCY);
	
	else if(current_time>2.35/FREQUENCY&&current_time<=2.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.65/FREQUENCY&&current_time<=2.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.65/FREQUENCY);
	
	else if(current_time>2.75/FREQUENCY&&current_time<=2.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.75/FREQUENCY);

///third T
	else if(current_time>2.85/FREQUENCY&&current_time<=3.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.15/FREQUENCY&&current_time<=3.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.15/FREQUENCY);
	
	else if(current_time>3.25/FREQUENCY&&current_time<=3.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.25/FREQUENCY);
	
	else if(current_time>3.35/FREQUENCY&&current_time<=3.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.65/FREQUENCY&&current_time<=3.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.65/FREQUENCY);
	
	else if(current_time>3.75/FREQUENCY&&current_time<=3.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.75/FREQUENCY);
///fourth T
	else if(current_time>3.85/FREQUENCY&&current_time<=4.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.15/FREQUENCY&&current_time<=4.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.15/FREQUENCY);
	
	else if(current_time>4.25/FREQUENCY&&current_time<=4.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.25/FREQUENCY);
	
	else if(current_time>4.35/FREQUENCY&&current_time<=4.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.65/FREQUENCY&&current_time<=4.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.65/FREQUENCY);
	
	else if(current_time>4.75/FREQUENCY&&current_time<=4.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.75/FREQUENCY);
	
	vel[1] =vv; 

/////angular velocity/////////////	
	if(current_time<=0.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	
	else if(current_time>0.5/FREQUENCY&&current_time<=1.0/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	
	else if(current_time>1.0/FREQUENCY&&current_time<=1.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

	else if(current_time>1.5/FREQUENCY&&current_time<=2.0/FREQUENCY)   
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
    
	else if(current_time>2.0/FREQUENCY&&current_time<=2.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

	else if(current_time>2.5/FREQUENCY&&current_time<=3.0/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	
        else if(current_time>3.0/FREQUENCY&&current_time<=3.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

	else if(current_time>3.5/FREQUENCY&&current_time<=4.0/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

    omega[0] =PI/180.0*tt;
	omega[1] =0.0 ;
	omega[2] = 0.0 ;
//	Message("time=%f  omega=%f\n",current_time,omega[0]) ; 
}

/************************************************************/
DEFINE_CG_MOTION(ARM2_nx_sin3,dt,vel,omega,time,dtime)
{
	double tt;
	double vv;
	current_time = CURRENT_TIME-Starttime ; 
	

	vel[0] = 0.0; //sin(PI*current_time)
	vel[2] = 0.0 ; 

	if(current_time<=0.15/FREQUENCY)
	vv = WALKVEL ; 
	
	else if(current_time>0.15/FREQUENCY&&current_time<=0.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.15/FREQUENCY);
	
	else if(current_time>0.25/FREQUENCY&&current_time<=0.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.25/FREQUENCY);
	
	else if(current_time>0.35/FREQUENCY&&current_time<=0.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>0.65/FREQUENCY&&current_time<=0.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.65/FREQUENCY);
	
	else if(current_time>0.75/FREQUENCY&&current_time<=0.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.75/FREQUENCY);
	
      
//////first T
	else if(current_time>0.85/FREQUENCY&&current_time<=1.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.15/FREQUENCY&&current_time<=1.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.15/FREQUENCY);
	
	else if(current_time>1.25/FREQUENCY&&current_time<=1.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.25/FREQUENCY);
	
	else if(current_time>1.35/FREQUENCY&&current_time<=1.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.65/FREQUENCY&&current_time<=1.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.65/FREQUENCY);
	
	else if(current_time>1.75/FREQUENCY&&current_time<=1.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.75/FREQUENCY);

////Second T
	else if(current_time>1.85/FREQUENCY&&current_time<=2.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.15/FREQUENCY&&current_time<=2.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.15/FREQUENCY);
	
	else if(current_time>2.25/FREQUENCY&&current_time<=2.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.25/FREQUENCY);
	
	else if(current_time>2.35/FREQUENCY&&current_time<=2.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.65/FREQUENCY&&current_time<=2.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.65/FREQUENCY);
	
	else if(current_time>2.75/FREQUENCY&&current_time<=2.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.75/FREQUENCY);

///third T
	else if(current_time>2.85/FREQUENCY&&current_time<=3.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.15/FREQUENCY&&current_time<=3.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.15/FREQUENCY);
	
	else if(current_time>3.25/FREQUENCY&&current_time<=3.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.25/FREQUENCY);
	
	else if(current_time>3.35/FREQUENCY&&current_time<=3.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.65/FREQUENCY&&current_time<=3.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.65/FREQUENCY);
	
	else if(current_time>3.75/FREQUENCY&&current_time<=3.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.75/FREQUENCY);
///fourth T
	else if(current_time>3.85/FREQUENCY&&current_time<=4.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.15/FREQUENCY&&current_time<=4.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.15/FREQUENCY);
	
	else if(current_time>4.25/FREQUENCY&&current_time<=4.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.25/FREQUENCY);
	
	else if(current_time>4.35/FREQUENCY&&current_time<=4.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.65/FREQUENCY&&current_time<=4.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.65/FREQUENCY);
	
	else if(current_time>4.75/FREQUENCY&&current_time<=4.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.75/FREQUENCY);
	
	vel[1] =vv; 


/////angular velocity/////////	

	if(current_time<=0.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	
	else if(current_time>0.5/FREQUENCY&&current_time<=1.0/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	
	else if(current_time>1.0/FREQUENCY&&current_time<=1.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

	else if(current_time>1.5/FREQUENCY&&current_time<=2.0/FREQUENCY)   
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
    
	else if(current_time>2.0/FREQUENCY&&current_time<=2.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

	else if(current_time>2.5/FREQUENCY&&current_time<=3.0/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	
        else if(current_time>3.0/FREQUENCY&&current_time<=3.5/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);

	else if(current_time>3.5/FREQUENCY&&current_time<=4.0/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time);
	

        omega[0] =PI/180.0*tt*0.75;
	omega[1] =0.0 ;
	omega[2] = 0.0 ;
//	Message("time=%f  omega=%f\n",current_time,omega[0]) ; 
}
/************************************************************/

DEFINE_CG_MOTION(LEG2_nx_sin3,dt,vel,omega,time,dtime)
{
    double tt;
	double vv;
	current_time = CURRENT_TIME-Starttime ; 
	

	vel[0] = 0.0; //sin(PI*current_time)
	vel[2] = 0.0 ; 

	if(current_time<=0.15/FREQUENCY)
	vv = WALKVEL ; 
	
	else if(current_time>0.15/FREQUENCY&&current_time<=0.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.15/FREQUENCY);
	
	else if(current_time>0.25/FREQUENCY&&current_time<=0.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.25/FREQUENCY);
	
	else if(current_time>0.35/FREQUENCY&&current_time<=0.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>0.65/FREQUENCY&&current_time<=0.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.65/FREQUENCY);
	
	else if(current_time>0.75/FREQUENCY&&current_time<=0.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.75/FREQUENCY);
	
      
//////first T
	else if(current_time>0.85/FREQUENCY&&current_time<=1.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.15/FREQUENCY&&current_time<=1.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.15/FREQUENCY);
	
	else if(current_time>1.25/FREQUENCY&&current_time<=1.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.25/FREQUENCY);
	
	else if(current_time>1.35/FREQUENCY&&current_time<=1.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.65/FREQUENCY&&current_time<=1.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.65/FREQUENCY);
	
	else if(current_time>1.75/FREQUENCY&&current_time<=1.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.75/FREQUENCY);

////Second T
	else if(current_time>1.85/FREQUENCY&&current_time<=2.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.15/FREQUENCY&&current_time<=2.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.15/FREQUENCY);
	
	else if(current_time>2.25/FREQUENCY&&current_time<=2.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.25/FREQUENCY);
	
	else if(current_time>2.35/FREQUENCY&&current_time<=2.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.65/FREQUENCY&&current_time<=2.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.65/FREQUENCY);
	
	else if(current_time>2.75/FREQUENCY&&current_time<=2.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.75/FREQUENCY);

///third T
	else if(current_time>2.85/FREQUENCY&&current_time<=3.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.15/FREQUENCY&&current_time<=3.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.15/FREQUENCY);
	
	else if(current_time>3.25/FREQUENCY&&current_time<=3.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.25/FREQUENCY);
	
	else if(current_time>3.35/FREQUENCY&&current_time<=3.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.65/FREQUENCY&&current_time<=3.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.65/FREQUENCY);
	
	else if(current_time>3.75/FREQUENCY&&current_time<=3.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.75/FREQUENCY);
///fourth T
	else if(current_time>3.85/FREQUENCY&&current_time<=4.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.15/FREQUENCY&&current_time<=4.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.15/FREQUENCY);
	
	else if(current_time>4.25/FREQUENCY&&current_time<=4.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.25/FREQUENCY);
	
	else if(current_time>4.35/FREQUENCY&&current_time<=4.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.65/FREQUENCY&&current_time<=4.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.65/FREQUENCY);
	
	else if(current_time>4.75/FREQUENCY&&current_time<=4.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.75/FREQUENCY);
	
	vel[1] =vv; 

//////angular velocity///////////

	if(current_time<=0.5/FREQUENCY) 
		
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);
	
	else if(current_time>0.5/FREQUENCY&&current_time<=1.0/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	
	else if(current_time>1.0/FREQUENCY&&current_time<=1.5/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	else if(current_time>1.5/FREQUENCY&&current_time<=2.0/FREQUENCY) 
	     tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

   
	else if(current_time>2.0/FREQUENCY&&current_time<=2.5/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	else if(current_time>2.5/FREQUENCY&&current_time<=3.0/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

        else if(current_time>3.0/FREQUENCY&&current_time<=3.5/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	else if(current_time>3.5/FREQUENCY&&current_time<=4.0/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);


        omega[0] =PI/180.0*tt;
	omega[1] =0.0 ;
	omega[2] = 0.0 ;
//	Message("time=%f  omega=%f\n",current_time,omega[0]) ; 
}


/************************************************************/
DEFINE_CG_MOTION(ARM1_px_sin3,dt,vel,omega,time,dtime)
{
    double tt;
	double vv;
	current_time = CURRENT_TIME-Starttime ; 
	

	vel[0] = 0.0; //sin(PI*current_time)
	vel[2] = 0.0 ; 

	if(current_time<=0.15/FREQUENCY)
	vv = WALKVEL ; 
	
	else if(current_time>0.15/FREQUENCY&&current_time<=0.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.15/FREQUENCY);
	
	else if(current_time>0.25/FREQUENCY&&current_time<=0.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.25/FREQUENCY);
	
	else if(current_time>0.35/FREQUENCY&&current_time<=0.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>0.65/FREQUENCY&&current_time<=0.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-0.65/FREQUENCY);
	
	else if(current_time>0.75/FREQUENCY&&current_time<=0.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-0.75/FREQUENCY);
	
      
//////first T
	else if(current_time>0.85/FREQUENCY&&current_time<=1.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.15/FREQUENCY&&current_time<=1.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.15/FREQUENCY);
	
	else if(current_time>1.25/FREQUENCY&&current_time<=1.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.25/FREQUENCY);
	
	else if(current_time>1.35/FREQUENCY&&current_time<=1.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>1.65/FREQUENCY&&current_time<=1.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-1.65/FREQUENCY);
	
	else if(current_time>1.75/FREQUENCY&&current_time<=1.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-1.75/FREQUENCY);

////Second T
	else if(current_time>1.85/FREQUENCY&&current_time<=2.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.15/FREQUENCY&&current_time<=2.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.15/FREQUENCY);
	
	else if(current_time>2.25/FREQUENCY&&current_time<=2.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.25/FREQUENCY);
	
	else if(current_time>2.35/FREQUENCY&&current_time<=2.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>2.65/FREQUENCY&&current_time<=2.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-2.65/FREQUENCY);
	
	else if(current_time>2.75/FREQUENCY&&current_time<=2.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-2.75/FREQUENCY);

///third T
	else if(current_time>2.85/FREQUENCY&&current_time<=3.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.15/FREQUENCY&&current_time<=3.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.15/FREQUENCY);
	
	else if(current_time>3.25/FREQUENCY&&current_time<=3.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.25/FREQUENCY);
	
	else if(current_time>3.35/FREQUENCY&&current_time<=3.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>3.65/FREQUENCY&&current_time<=3.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-3.65/FREQUENCY);
	
	else if(current_time>3.75/FREQUENCY&&current_time<=3.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-3.75/FREQUENCY);
///fourth T
	else if(current_time>3.85/FREQUENCY&&current_time<=4.15/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.15/FREQUENCY&&current_time<=4.25/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.15/FREQUENCY);
	
	else if(current_time>4.25/FREQUENCY&&current_time<=4.35/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.25/FREQUENCY);
	
	else if(current_time>4.35/FREQUENCY&&current_time<=4.65/FREQUENCY)
	vv = WALKVEL;

	else if(current_time>4.65/FREQUENCY&&current_time<=4.75/FREQUENCY)
	vv = WALKVEL-9*WALKVEL*FREQUENCY*(current_time-4.65/FREQUENCY);
	
	else if(current_time>4.75/FREQUENCY&&current_time<=4.85/FREQUENCY)
	vv = 0.1*WALKVEL+9*WALKVEL*FREQUENCY*(current_time-4.75/FREQUENCY);
	
	vel[1] =vv; 


////////angular velocity///////////

	if(current_time<=0.5/FREQUENCY) 
		
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);
	
	else if(current_time>0.5/FREQUENCY&&current_time<=1.0/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	
	else if(current_time>1.0/FREQUENCY&&current_time<=1.5/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	else if(current_time>1.5/FREQUENCY&&current_time<=2.0/FREQUENCY) 
	     tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

   
	else if(current_time>2.0/FREQUENCY&&current_time<=2.5/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	else if(current_time>2.5/FREQUENCY&&current_time<=3.0/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

     else if(current_time>3.0/FREQUENCY&&current_time<=3.5/FREQUENCY) 
		tt=Hipmin*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);

	else if(current_time>3.5/FREQUENCY&&current_time<=4.0/FREQUENCY) 
		tt=Hipmax*2*PI*FREQUENCY*cos(2*PI*FREQUENCY*current_time+PI);



    omega[0] =PI/180.0*tt*0.75;
	omega[1] =0.0 ;
	omega[2] = 0.0 ;
//	Message("time=%f  omega=%f\n",current_time,omega[0]) ; 
}

/************************************************************/

/************************************************************/
