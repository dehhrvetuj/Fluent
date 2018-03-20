/* Simulation of Coal Ash Deposition on Modern Turbine Nozzle Guide Vanes  - Master Thesis */

#include "udf.h"
#include "cxiface.h"
DEFINE_DPM_INJECTION_INIT(injection_velocity,I)
{
Particle *p;
loop(p,I->p) /* Standard Fluent Looping Macro to get
particle streams in an Injection */
{
cell_t c;
Thread *t;
CX_Cell_Id cx_cell;
cx_cell=*CX_Find_Cell_With_Point(P_POS(p));
c=RP_CELL(&cx_cell);
t=RP_THREAD(&cx_cell);
P_VEL(p)[0] = C_U(c,t);
P_VEL(p)[1] = C_V(c,t);
#if RP_3D
P_VEL(p)[2] = C_W(c,t);
#endif
P_T(p)=C_T(c,t);
}
}

/*

cell_t c;
Thread *t;
CX_Cell_Id cx_cell;
real NV_VEC(pt);
real c_centroid[ND_ND];

NV_D(pt, =, 1,1,1);  //coordinate of your specified location,it must be in the domain coordinate range
CX_Start_ND_Point_Search();
cx_cell=*CX_Find_Cell_With_Point(pt);
CX_End_ND_Point_Search();
c=RP_CELL(&cx_cell);  //the right cell number
t = RP_THREAD(&cx_cell);  //  the thread

C_CENTROID(c_centroid,c,t);
Message0("coordinate of the specified point: x=%g,y=%g,z=%g\n",pt[0],pt[1],pt[2]);
Message0("coordinate of the cell found: x=%g,y=%g,z=%g,T=%g\n",c_centroid[0],c_centroid[1],c_centroid[2],C_T(c,t));

*/
