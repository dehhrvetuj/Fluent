#include "udf.h"

#define CG_DISTANCE 0.5


static real v_prev = 0.0;

DEFINE_CG_MOTION(flap,dt,vel,omega,time,dtime)
{
	vel[1] = -0.3;
}


#define C 1000.0

int IsPorous(real x[ND_ND])
{
  if ((x[0]>=0.2 && x[0]<=0.3) && (x[1]<=1.6 || x[1]>=2.2 || x[2]>=1.8 || x[2]<=0))
  {
	return 1;
  }
  else
  {
	return 0;
  }	
} 


DEFINE_SOURCE(xmom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real con, source;

  C_CENTROID(x,c,t);
  con = C*0.5*C_R(c,t);

  if (IsPorous(x))
  {
	source = -con*fabs(C_U(c, t))*C_U(c,t);
	dS[eqn] = -2.*con*fabs(C_U(c,t));  
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;
  }


  return source;
}

DEFINE_SOURCE(ymom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real con, source;

  C_CENTROID(x,c,t);
  con = C*0.5*C_R(c,t);

  if (IsPorous(x))
  {
	  source = -con*fabs(C_V(c, t))*C_V(c,t);
	  dS[eqn] = -2.*con*con*fabs(C_V(c,t));	  
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;  
  }


  return source;
}

DEFINE_SOURCE(zmom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real con, source;

  C_CENTROID(x,c,t);
  con = C*0.5*C_R(c,t);

  if (IsPorous(x))
  {
	source = -con*fabs(C_W(c, t))*C_W(c,t);
	dS[eqn] = -2.*con*fabs(C_W(c,t));	  
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;    
  }


  return source;
}

