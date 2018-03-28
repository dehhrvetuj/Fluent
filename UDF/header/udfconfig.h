/*
 *
 * Copyright 2003-2011 ANSYS, Inc.
 * All Rights Reserved
 */

#define UDF_COMPILER 1

#define EOF (-1)
#define NULL (0)

#define const
#define register


typedef unsigned long size_t;

typedef long int64_t;



/* config.h */

#define RP_2D 0
#define RP_3D 1
#define RP_HOST 0
#define RP_NODE 0
#define RP_DOUBLE 1
#define RP_NETWORK 1

#define STRUCT_OFF(t,offset,type)((type*)((char *)(t)+offset))
#define STRUCT_REF(t,offset,type)(*STRUCT_OFF(t,offset,type))

#define DOMAIN_C(t)STRUCT_REF(t,0,Thread *)
#define DOMAIN_NOSOLVE_C(t)STRUCT_REF(t,8,Thread *)
#define DOMAIN_NOSOLVE_EXT_C(t)STRUCT_REF(t,16,Thread *)
#define DOMAIN_F(t)STRUCT_REF(t,24,Thread *)
#define DOMAIN_NOSOLVE_F(t)STRUCT_REF(t,32,Thread *)
#define DOMAIN_ID(t)STRUCT_REF(t,2376,int)
#define PHASE_DOMAIN_INDEX(t)STRUCT_REF(t,2380,int)
#define DOMAIN_N_DOMAINS(t)STRUCT_REF(t,2388,int)
#define DOMAIN_SUPER_DOMAIN(t)STRUCT_REF(t,2568,Domain *)
#define DOMAIN_SUB_DOMAIN(t,i)(STRUCT_OFF(t,2392,Domain *)[i])

#define THREAD_STORE(t,nv)(STRUCT_OFF(t,62312,struct store_struct *)[nv])
#define THREAD_STORAGE(t,nv)(STRUCT_OFF(t,912,void *)[nv])
#define THREAD_T0(t)STRUCT_REF(t,344,Thread *)
#define THREAD_T1(t)STRUCT_REF(t,352,Thread *)
#define THREAD_NEXT(t)STRUCT_REF(t,488,Thread *)
#define THREAD_SHADOW(t)STRUCT_REF(t,496,Thread *)
#define THREAD_ID(t)STRUCT_REF(t,832,int)
#define THREAD_NAME(t)STRUCT_REF(t,72,char *)
#define THREAD_N_ELEMENTS(t)STRUCT_REF(t,332,int)
#define THREAD_FLAGS(t)STRUCT_REF(t,836,int)
#define THREAD_SUB_THREAD(t,i)(STRUCT_OFF(t,584,Thread *)[i])
#define THREAD_SUB_THREADS(t)STRUCT_OFF(t,584,Thread *)
#define THREAD_SUPER_THREAD(t)STRUCT_REF(t,552,Thread *)
#define THREAD_DOMAIN(t)STRUCT_REF(t,904,Domain *)
#define THREAD_HEAD(t)STRUCT_REF(t,0,Thread_Head *)
#define THREAD_MATERIAL(t)(STRUCT_REF(THREAD_HEAD(t),144728,Material *))

#define NODE_COORD(v)STRUCT_OFF(v,24,double)

#define C_NFACES(c,t) STRUCT_OFF(C_CELL (c,t), 12, unsigned int)

#define P_INIT_DIAM(p)STRUCT_REF(p,792,real)
#define P_INIT_MASS(p)STRUCT_REF(p,808,real)
#define P_INIT_RHO(p)STRUCT_REF(p,800,real)
#define P_INIT_TEMP(p)STRUCT_REF(p,816,real)
#define P_INIT_LMF(p)STRUCT_REF(p,832,real)
#define P_VF(p)STRUCT_REF(p,2096,real)
#define P_CF(p)STRUCT_REF(p,2104,real)
#define P_VFF(p)STRUCT_REF(p,2112,real)
#define P_DF(p)STRUCT_REF(p,2120,real)
#define P_DT(p)STRUCT_REF(p,72,real)
#define P_DIAM(p)STRUCT_REF(p,984,real)
#define P_T(p)STRUCT_REF(p,1008,real)
#define P_VEL(p)STRUCT_OFF(p,960,real)
#define P_RHO(p)STRUCT_REF(p,992,real)
#define P_LMF(p)STRUCT_REF(p,1024,real)
#define P_MASS(p)STRUCT_REF(p,1000,real)
#define P_TIME(p)STRUCT_REF(p,1016,real)
#define P_DIAM0(p)STRUCT_REF(p,888,real)
#define P_VEL0(p)STRUCT_OFF(p,864,real)
#define P_T0(p)STRUCT_REF(p,912,real)
#define P_RHO0(p)STRUCT_REF(p,896,real)
#define P_LMF0(p)STRUCT_REF(p,928,real)
#define P_MASS0(p)STRUCT_REF(p,904,real)
#define P_TIME0(p)STRUCT_REF(p,920,real)
#define P_Y_S(p)STRUCT_REF(p,576,real)
#define P_E_IMP(p)STRUCT_REF(p,584,real)
#define P_WEBER_IMP(p)STRUCT_REF(p,592,real)
#define P_CURRENT_LAW_INDEX(p)STRUCT_REF(p,540,int)
#define P_INJECTION(p)STRUCT_REF(p,728,Injection *)
#define P_EVAP_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),5288,int)
#define P_DEVOL_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),5292,int)
#define P_OXID_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),5296,int)
#define P_PROD_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),5300,int)
#define P_LAWS(p,in)STRUCT_OFF(P_INJECTION(p),780,int)[in]

typedef char UDF_Cache_Data[48];

#define MATERIAL_PROPERTY(m)STRUCT_OFF(m,128,Property)
#define PROPERTY_LENGTH 200
#define MATERIAL_PROPERTY_ELEMENT(m,i)((Property *)((char *)MATERIAL_PROPERTY(m)+PROPERTY_LENGTH*i))
#define MATERIAL_PROP(m,i)(STRUCT_REF(MATERIAL_PROPERTY_ELEMENT(m,i),8,float))

#define NW_CELL(n) STRUCT_REF(((char *)network+n*24),16,Network_cell *) 
#define NWC_T(n,c) STRUCT_REF(((char *)NW_CELL(n)+c*488),256,real) 
#define NWE_T(e) STRUCT_REF(((char *)nw_end + e*264),8,real)
