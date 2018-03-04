/* Although FLUENT provides a method of sampling DPM particles as they pass through planes and outlets, 
there is no simple way to export particle data for all of the particles in the domain. 
This is especially difficult when running FLUENT in parallel since the particles are located on different compute nodes. 
The UDF below can be used to loop through all of the particle in the domain and write selected particle properties to a file. 
The example shows how to write the diameter, mass, and temperature of the particles, 
but any properties associated with the DPM particle structure can be exported. 
*/

/* dpm_sample.c */ 
/* */ 
/* Loop over all particles in domain */ 
/* */ 
/* FLUENT 6.3 */ 

#include "udf.h" 

#define WRITE_FILE TRUE /* text file created if true */ 

DEFINE_ON_DEMAND(all_unsteady_particles) 
{ 
  Injection *I; 
  Injection *dpm_injections = Get_dpm_injections(); 
  Particle *p; 
  int counter = 0; 
  #if WRITE_FILE 
  FILE *fyle; 

  fyle = par_fopen("parcels.out","a",1,1); 

  par_fprintf_head(fyle," DIAMETER MASS TEMPERATURE n"); 
  #endif /* WRITE_FILE */ 

  loop(I,dpm_injections) 
  { 
    loop(p,I->p) 
    { 
    counter++; 

    #if WRITE_FILE 
    /* data to be written to file goes here */ 
    #if PARALLEL 
    par_fprintf(fyle,"%d, %e %e %e n",I->try_id,P_DIAM(p),P_MASS(p),P_T(p)); 
    #else 
    par_fprintf(fyle,"%e %e %e n",P_DIAM(p),P_MASS(p),P_T(p)); 
    #endif /* PARALLEL */ 
    #endif /* WRITE_FILE */ 
    } 
  } 

  #if WRITE_FILE 
  par_fclose(fyle); 
  #endif 

  counter = PRF_GRSUM1(counter); 

  Message0("There are %d dpm parcels in the domain. n",counter); 
} 
