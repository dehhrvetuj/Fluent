#include "udf.h"
 
#define FACE_ZONE_ID 6

/* The following is the serial version */
DEFINE_ON_DEMAND(pres_check)
{
    int ID;
    face_t f;
    real maxPressure;
    Thread *thread;
    Domain *d;

    ID = FACE_ZONE_ID;
    maxPressure = 1.0;

    d = Get_Domain(1);

    thread = Lookup_Thread(d, ID);

    begin_f_loop(f, thread)
    {
        if (F_P(f, thread) > maxPressure) 
            maxPressure = F_P(f, thread);
    }
    end_f_loop(f, thread)

    printf("Max Pressure = %f \n",maxPressure);

    RP_Set_Real("pressure_on_wall",maxPressure);
}


/* The following is the parallel version */
DEFINE_ON_DEMAND(pres_check)
{
    int ID = 0;
    real maxPressure = 0.0;

    face_t f;
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
    #if RP_NODE     /* only parallel compute nodes involved here; perform node synchronized action here */        
        maxPressure = PRF_GRHIGH1(maxPressure);
    #endif

    Message0("\nMax Pressure = %f \n",maxPressure);   /* message sent by node-0 or serial */

    /* In parallel, send the combined max pressure to the host: */
    node_to_host_real_1(maxPressure); /* do nothing in serial */

    /* In parallel, only host or serial can deal with RP variables: */
    #if !RP_NODE    /* serial or host */
    if(RP_Variable_Exists_P("pressure_on_wall")) 
    {
        RP_Set_Real("pressure_on_wall",maxPressure);
    }
    else
    {
        Message("\nThe UDF would like to set the RP-var 'pressure_on_wall\n");
        Message("but this RP-var does not currently exist.\n");
    }  
    #endif

}
