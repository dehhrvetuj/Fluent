#include "udf.h"

#define CG_DISTANCE 0.5


static real v_prev = 0.0;

DEFINE_CG_MOTION(flap,dt,vel,omega,time,dtime)
{
	vel[1] = -0.3;
}


#define C1 1.0
#define C2 1000.0

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

void Porousity(real v, real rho, real mu, real* source, real* dS)
{
	real C_l = 0.0;
	real C_q = 0.0;
	
	C_l = C1*mu;
	C_q = C2*rho*fabs(v)*0.5;
	
	*source = - (C_l + C_q) * v;
	*dS = - (C_l + 2*C_q);
}

DEFINE_SOURCE(xmom_source,c,t,dS,eqn)
{
  real x[ND_ND];
  real source;

  C_CENTROID(x,c,t);

  if (IsPorous(x))
  {
	Porousity(C_U(c,t), C_R(c,t), C_MU_L(c,t), &source, &(dS[eqn]));
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
  real source;

  C_CENTROID(x,c,t);

  if (IsPorous(x))
  {
	Porousity(C_V(c,t), C_R(c,t), C_MU_L(c,t), &source, &(dS[eqn]));
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
  real source;

  C_CENTROID(x,c,t);

  if (IsPorous(x))
  {
	Porousity(C_W(c,t), C_R(c,t), C_MU_L(c,t), &source, &(dS[eqn]));
  }
  else
  {
	source = 0.0;
	dS[eqn] = 0.0;
  }


  return source;
}

