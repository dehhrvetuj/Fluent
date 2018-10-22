#include "udf.h"
#include "stdlib.h"
#include "var.h"

#ifdef STRUCT_REF
#define PRINT printf
#define thread-id-list a
#define thread-name-list b
#define abs-pressure p
#else
#define PRINT CX_Message
#endif

DEFINE_ON_DEMAND(zoneid_zonename)
{
    /*print thread id_s, thread_names and volume-integral values of abs-pressure for each thread*/
    int i;
    real p_operating=RP_Get_Float("operating-pressure");/*operating pressure*/
    int a = RP_Get_List_Length("a");
    int b = RP_Get_List_Length("b");
    PRINT("length of ID list: %d\n", a);
    PRINT("length of NAME list: %d\n", b);
    Domain *domain;
    domain = Get_Domain(1); /*assuming not multiphase flow, there is only one domain with number=1*/
    Thread *c_thread;

    int threads=0;
    thread_loop_c(c_thread, domain)
    {
    threads++; /*count cell threads number in our calculation domain*/
    }

    real* p = (double*)malloc(threads * sizeof(double*));/*array of volume-integral values of abs-pressure for each thread*/
    for (i = 0; i < threads; i++)
        p[i]=0.0;
    real abs_pressure;
    real total_volume;

    thread_loop_c(c_thread, domain)/*here we loop over all cell threads in domain*/
    {
     abs_pressure=0.0;
     total_volume=0.0;
     for (i = 0; i < a; i++)
         if (RP_Get_List_Ref_Int("a", i)==THREAD_ID(c_thread))
            {    
                cell_t c;
                begin_c_loop(c, c_thread)/*we should loop over all cells in the thread*/
                  {
                    abs_pressure+=(C_P(c,c_thread)+p_operating)*C_VOLUME(c,c_thread);
                    total_volume+=C_VOLUME(c,c_thread);
                  }                         
                end_c_loop(c, c_thread)
                abs_pressure/=total_volume;
                p[i]=abs_pressure;
                break;
            }
    }

    for (i = 0; i < a; i++)
    {
        PRINT("Zone ID: %d ...has the name %s... has the pressure = %f\n",
        RP_Get_List_Ref_Int("a", i),
        RP_Get_List_Ref_String("b", i),
        p[i]);
    }
    free(p);
}
