/**********************************************************************
   Concatenated UDFs for the Discrete Phase Model that includes a     
   usage of DPM_SWITCH                                                
***********************************************************************/

#include "udf.h"
#include "dpm.h"
#include "surf.h"  /* for  macros: RP_Cell() & RP_Thread()        */
#include "prop.h"  /* for function: Saturation_Pressure() (of water) */

static int counter=0;
static real dpm_relax=1.0; /*dpm source relaxation */

real H2O_Saturation_Pressure(real T)
{
  real ratio, aTmTp;

  aTmTp = .01 * (T - 338.15);
  ratio = (647.286/T - 1.) *
    (-7.419242 + aTmTp*(.29721 +
                 aTmTp*(-.1155286 +
                 aTmTp*(8.685635e-3 +
                 aTmTp*(1.094098e-3 +
                 aTmTp*(-4.39993e-3 +
                 aTmTp*(2.520658e-3 -
                 aTmTp*5.218684e-4)))))));
  return (22.089e6 * exp(MIN(ratio,35.)));
}


real myHumidity(cell_t c,Thread *t)
{
  int i;
  Material *m=THREAD_MATERIAL(t), *sp;
  real yi_h2o,mw_h2o;
  real r_mix=0.0;

  if(MATERIAL_TYPE(m)==MATERIAL_MIXTURE)
  {
   mixture_species_loop (m,sp,i)
   {
     r_mix += C_YI(c,t,i)/MATERIAL_PROP(sp,PROP_mwi);

     if (0 == strcmp(MIXTURE_SPECIE_NAME(m,i),"h2o") ||
  (0 == strcmp(MIXTURE_SPECIE_NAME(m,i),"H2O")))
     {
 yi_h2o = C_YI(c,t,i);
 mw_h2o = MATERIAL_PROP(sp,PROP_mwi);
     }
   }
  }

  return ((ABS_P(C_P(c,t),op_pres) * yi_h2o / (mw_h2o * r_mix)) /
                H2O_Saturation_Pressure(C_T(c,t))) ;
}

#define CONDENS 1.0e-4

DEFINE_DPM_LAW(condenshumidlaw,p,coupled)
{
 real area;
 real mp_dot;
 cell_t c = P_CELL(p);               /* Get Cell and Thread from */
 Thread *t = P_THREAD(p);  /* Particle Structure using new macros*/

 area = 4.0* M_PI * (P_DIAM(p)*P_DIAM(p));

 /* Note This law only used if Humidity > 1.0 so mp_dot always positive*/
 mp_dot = CONDENS*sqrt(area)*(myHumidity(c,t)-1.0);

 if(mp_dot>0.0)
 {
   P_MASS(p) = P_MASS(p) + mp_dot*P_DT(p);
   P_DIAM(p) = pow(6.0*P_MASS(p)/(P_RHO(p)* M_PI), 1./3.);
   P_T(p)=C_T(c,t); /* Assume condensing particle is in thermal
          equilibrium with fluid in cell */
 }

}

/* define macro that is not yet standard */

#define C_DPMS_ENERGY(c,t)C_STORAGE_R(c,t,SV_DPMS_ENERGY)

DEFINE_DPM_SOURCE(dpm_source,c,t,S,strength,p)
{
  real mp_dot;
  Material *sp = P_MATERIAL(p);


  /* mp_dot is the (positive) mass source to the continuous phase */
  /* (Difference in mass between entry and exit from cell)         */
  /* multiplied by strength (Number of particles/s in stream)      */

  mp_dot = (P_MASS0(p) - P_MASS(p)) * strength;

   C_DPMS_YI(c,t,0) += mp_dot*dpm_relax;
   C_DPMS_ENERGY(c,t) -= mp_dot*dpm_relax*
MATERIAL_PROP(sp,PROP_Cp)*(C_T(c,t)-298.15);
   C_DPMS_ENERGY(c,t) -= mp_dot*dpm_relax*
MATERIAL_PROP(sp,PROP_latent_heat);
}


#define UDM_RH 0
#define N_REQ_UDM 1
#define CONDENS_LIMIT 1.0e-10

DEFINE_DPM_SWITCH(dpm_switch,p,coupled)
{
 cell_t c = RP_CELL(&p->cCell);
 Thread *t = RP_THREAD(&p->cCell);

 if(C_UDMI(c,t,UDM_RH) > 1.0)
  P_CURRENT_LAW(p) = DPM_LAW_USER_1;
 else
 {
  if(P_MASS(p) < CONDENS_LIMIT)
   P_CURRENT_LAW(p) = DPM_LAW_INITIAL_INERT_HEATING;
  else
   P_CURRENT_LAW(p) = DPM_LAW_VAPORIZATION;
 }
}

DEFINE_ADJUST(adj_relhum,domain)
{
 cell_t cell;
 Thread *thread;

 /* set dpm source underrelaxation */
 dpm_relax = Domainvar_Get_Real(ROOT_DOMAIN_ID,"dpm/relax");

 if(sg_udm<N_REQ_UDM)
    Message("\nNot enough user defined memory allocated. %d required.\n",
            N_REQ_UDM);
 else
 {
  real humidity,min,max;

  min=1e10;
  max=0.0;

  thread_loop_c(thread,domain)
  {
   /* Check if thread is a Fluid thread and has UDMs set up on it */
   if (FLUID_THREAD_P(thread)&& NNULLP(THREAD_STORAGE(thread,SV_UDM_I)))
   {
    begin_c_loop(cell,thread)
     humidity=myHumidity(cell,thread);
     min=MIN(min,humidity);
     max=MAX(max,humidity);
     C_UDMI(cell,thread,UDM_RH)=humidity;
    end_c_loop(cell,thread)
   }
  }
  Message("\nRelative Humidity set in udm-%d
range:(%f,%f)\n",UDM_RH,min,max);
 }/* end if for enough UDSs and UDMs */
}

DEFINE_ON_DEMAND(set_relhum)
{
 adj_relhum(Get_Domain(1));
}
