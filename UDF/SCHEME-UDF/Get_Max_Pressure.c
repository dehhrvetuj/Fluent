#include "udf.h"
 
#define FACE_ZONE_ID 6

DEFINE_ON_DEMAND(pres_check)
{
  int ID;
  face_t f;
  real maxPressure;
  Thread *thread;
  Domain *d;

  maxPressure = -9.99e30;

  d = Get_Domain(1);

  thread = Lookup_Thread(d, FACE_ZONE_ID);

  begin_f_loop(f, thread)
  {
    if (F_P(f, thread) > maxPressure)
      maxPressure = F_P(f, thread);
  }
  end_f_loop(f, thread)

  /* In parallel, get the combined max pressure across all nodes: */
#if RP_NODE
  maxPressure = PRF_GRHIGH1(maxPressure);
#endif

  Message0("\nMax Pressure = %f \n",maxPressure);

  /* In parallel, send the combined max pressure to the host: */
  node_to_host_real_1(maxPressure);

  /* In parallel, only the host sets RP variables: */
#if !RP_NODE
  if(RP_Variable_Exists_P("pressure_on_wall")) {
    RP_Set_Real("pressure_on_wall",maxPressure);
  }else{
    Message("\nThe UDF would like to set the RP-var 'pressure_on_wall\n");
    Message("but this RP-var does not currently exist.\n");
  }
#endif

}
