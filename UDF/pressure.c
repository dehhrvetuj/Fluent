#include "udf.h"
#define SET_POINT 0.005
DEFINE_PROFILE(outlet_pressure, thread, nv)
{
Thread *t;
face_t f;
cell_t c;
real actual_mass_flow;
real pressure = C_P(c,t);

actual_mass_flow = F_FLUX(f,t);

if (actual_mass_flow > SET_POINT)
begin_f_loop (f,t)
{
F_PROFILE(f,thread,nv) = pressure + 1;
}
end_f_loop (f,t)

else if (actual_mass_flow < SET_POINT)
begin_f_loop (f,t)
{
F_PROFILE(f,thread,nv) = pressure - 1;
}
end_f_loop (f,t)

else
begin_f_loop (f,t)
{
F_PROFILE(f,thread,nv) = pressure;
}
end_f_loop (f,t)
}


#include "udf.h"

/* Average temperature is calculated and share with other CPUs */

DEFINE_ADJUST(average_exit_temp, domain)
{
#if !RP_HOST    /*  either serial or compute node process is involved */

	face_t f;
	real sum_temp=0.0;  
	int nfaces = 0, i; 
	int ID  = 21;	//on_hava_cikisi
	int ID0 = 31;	//uflec_orta_sag
	int ID1 = 30;	//uflec_orta_sol
	int ID2 = 28;	//uflec_sag
	int ID3 = 29;	//uflec_sag_cam
	int ID4 = 26;	//uflec_sol
	int ID5 = 27;	//uflec_sol_cam

	/* Determination of zone IDs */
	Thread *thread  = Lookup_Thread(domain, ID );
	Thread *thread0 = Lookup_Thread(domain, ID0);
	Thread *thread1 = Lookup_Thread(domain, ID1);
	Thread *thread2 = Lookup_Thread(domain, ID2);
	Thread *thread3 = Lookup_Thread(domain, ID3);
	Thread *thread4 = Lookup_Thread(domain, ID4);
	Thread *thread5 = Lookup_Thread(domain, ID5);

	/* Calculation of average exit temperature and sending to other nodes. The thread on which the average temperature is calculated is on node zero*/
	if(I_AM_NODE_ZERO_P)
	{
		begin_f_loop(f, thread)
		{
			nfaces = nfaces + 1;
			sum_temp = sum_temp + F_T(f,thread);
		}
		end_f_loop(f,thread)
		sum_temp = sum_temp/nfaces;
		Message("Node %d :  Average exit temperature = %f\n", myid, sum_temp);
		compute_node_loop_not_zero(i)
		{
			PRF_CSEND_REAL(i, &sum_temp, 1, myid);
		}
	}

	/* Reception of average temperature from node zero */
	if(! I_AM_NODE_ZERO_P)
	{
		PRF_CRECV_REAL(0, &sum_temp, 1, 0);
		Message("Node %d : Average exit temperature = %f\n", myid, sum_temp);
	}

	/* User defined memory of thread0 are filled with sum_temp */
	begin_f_loop(f, thread0)
	{
		F_UDMI(f,thread0,0) = sum_temp;
	}
	end_f_loop(f,thread0)

	/* User defined memory of thread1 are filled with sum_temp */
	begin_f_loop(f, thread1)
	{
		F_UDMI(f,thread1,1) = sum_temp;
	}
	end_f_loop(f,thread1)

	/* User defined memory of thread2 are filled with sum_temp */
	begin_f_loop(f, thread2)
	{
		F_UDMI(f,thread2,2) = sum_temp;
	}
	end_f_loop(f,thread2)

	/* User defined memory of thread3 are filled with sum_temp */
	begin_f_loop(f, thread3)
	{
		F_UDMI(f,thread3,3) = sum_temp;
	}
	end_f_loop(f,thread3)

	/* User defined memory of thread4 are filled with sum_temp */
	begin_f_loop(f, thread4)
	{
		F_UDMI(f,thread4,4) = sum_temp;
	}
	end_f_loop(f,thread4)

	/* User defined memory of thread5 are filled with sum_temp */
	begin_f_loop(f, thread5)
	{
		F_UDMI(f,thread5,5) = sum_temp;
	}
	end_f_loop(f,thread5)

#endif 
}

/* Boundary conditions are filled from user defined memory locations  */ 

DEFINE_PROFILE(temp_uflec_orta_sag,t,i)
{
	#if !RP_HOST
	face_t f;

	begin_f_loop(f,t)
	{
		F_PROFILE(f,t,i) = F_UDMI(f,t,0);
	}
	end_f_loop(f,t)
	#endif 
}

DEFINE_PROFILE(temp_uflec_orta_sol,t,i)
{
	#if !RP_HOST
	face_t f;

	begin_f_loop(f,t)
	{
		F_PROFILE(f,t,i) = F_UDMI(f,t,1);
	}
	end_f_loop(f,t)
	#endif 
}

DEFINE_PROFILE(temp_uflec_sag,t,i)
{
	#if !RP_HOST
	face_t f;

	begin_f_loop(f,t)
	{
		F_PROFILE(f,t,i) = F_UDMI(f,t,2);
	}
	end_f_loop(f,t)
	#endif 
}

DEFINE_PROFILE(temp_uflec_sag_cam,t,i)
{
	#if !RP_HOST
	face_t f;

	begin_f_loop(f,t)
	{
		F_PROFILE(f,t,i) = F_UDMI(f,t,3);
	}
	end_f_loop(f,t)
	#endif 
}

DEFINE_PROFILE(temp_uflec_sol,t,i)
{
	#if !RP_HOST
	face_t f;

	begin_f_loop(f,t)
	{
		F_PROFILE(f,t,i) = F_UDMI(f,t,4);
	}
	end_f_loop(f,t)
	#endif 
}

DEFINE_PROFILE(temp_uflec_sol_cam,t,i)
{
	#if !RP_HOST
	face_t f;

	begin_f_loop(f,t)
	{
		F_PROFILE(f,t,i) = F_UDMI(f,t,5);
	}
	end_f_loop(f,t)
	#endif 
}