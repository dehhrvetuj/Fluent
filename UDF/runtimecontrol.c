#include "udf.h"

/* The RP_Set_List_of_Integer function does not in V6.1 but is in V6.2 */ /* Remove next line and list_of_integer.[ch] files for V6.2 */ #include "list_of_integer.h"

#define N_SWITCHES 4

#define UNKNOWN 0 #define LEAVE 0 #define CLOSED 1 #define OPENED 2

DEFINE_ON_DEMAND(set_switches) { #if !RP_NODE /* only does things on HOST or SERIAL */

static int states[N_SWITCHES] = {UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN};

real shifts[N_SWITCHES] = {0.0, 90.0, 270.0, 180.0};

int i, change, switches[N_SWITCHES]; real rpm = 6000.0; real time, period, phase, shift_phase;

period = 60.0/rpm; /* seconds */ time=CURRENT_TIME;

Message("\n-----------------------------------------------\n");

period = 60.0/rpm; /* seconds */

time= CURRENT_TIME + CURRENT_TIMESTEP; /* time at NEXT time step */ Message("Time is %f(s)\n",time);

phase=time/period; phase=360.0*(phase-floor(phase)); /* degrees */ Message("Phase is %f(degs)\n",phase);

for (i=0;i<N_SWITCHES;i++)

{

shift_phase = phase + shifts[i];

while(shift_phase >= 360.0) /* refix modulo after addition of shift */

shift_phase -= 360.0;

if(shift_phase< 80.0)

change=OPENED;

else

change=CLOSED;

if(states[i]==change)

switches[i]=LEAVE;

else

{

states[i]=change;

switches[i]=change;

}

}

RP_Set_List_of_Integer("udf-switches", switches, N_SWITCHES);

Message("-----------------------------------------------\n"); #endif /* !RP_NODE */ }

DEFINE_PROFILE(Mprof, ft, m) { face_t f;

begin_f_loop(f, ft)

{

F_PROFILE(f, ft, m) = 250;

} end_f_loop(f, ft) }

/* list_of_integer.c */ #include "cxserv.h"

static void rpsetvar(char *s, Pointer val) { Pointer p; stack *oldstk = Save_Stack(); extern Pointer sym_quote; Pointer sym_uienv; Pointer sym_rpsetvarlocal;

sym_uienv = intern("user-initial-environment"); sym_rpsetvarlocal = intern("rpsetvarlocal");

Push_Stack0(val); /* protect val from garbage collection */ /* (eval `(rpsetvarlocal ',s ',v) user-initial-environment) */ Push_Stack(p); p = cons(NIL,NIL); CAR(p) = cons(val,NIL); CAR(p) = cons(sym_quote,CAR(p)); p = cons(NIL,p); CAR(p) = cons(intern(s),NIL); CAR(p) = cons(sym_quote,CAR(p)); p = cons(sym_rpsetvarlocal,p); (void) eval(p, eval(sym_uienv,NIL)); Restore_Stack(oldstk); }

void RP_Set_List_of_Integer(char *s, int a[], int len) { int n; Pointer result = NIL; stack *oldstk;

if (len <= 0) err("RP_Set_List_of_Integer: zero length list", NIL);

oldstk = Save_Stack(); Push_Stack(result);

for (n=len-1; n>=0; n--)

{

result = cons(NIL,result);

CAR(result) = fixcons(a[n]);

}

rpsetvar(s,result); Restore_Stack(oldstk); }

/* list_of_integer.h */ void RP_Set_List_of_Integer(char *s, int a[], int len);

;; switch.scm ;; load this file into fluent so that (run-switched nts) is available

(if (not (rp-var-object 'udf-switches))

(rp-var-define 'udf-switches () 'list #f))

(define run-switched/stop-filename "run-switched-stop.txt")

(define (run-switched . t-steps) (if

(do ((i (car t-steps) (- i 1)))

((or (= i 0) (file-exists? run-switched/stop-filename)) (> i 0))

(let ((list)(len)(state))

(format "~%Time Steps Remaining ~d~%" i)

(%udf-on-demand "set_switches")

(set! list (%rpgetvar 'udf-switches))

(set! len (length (%rpgetvar 'udf-switches)))

(do ((j 1 (+ j 1))) ((> j len) j)

(set! state (car list))

(set! list (cdr list))

(if (= state 1)

(ti-menu-load-string

(format #f "/def/bc/mz/zone-type/mass_flow_inlet.~d wall~%" j)))

(if (= state 2)

(begin

(ti-menu-load-string

(format #f "/def/bc/mz/zone-type/ mass_flow_inlet.~d mass-flow-inlet~%" j))

(ti-menu-load-string

(format #f "/def/bc/mfi mass_flow_inlet.~d n y y \"udf\" \"Mprof\" n 330 n 0 y y y n 1 n 0 n 0~%" j))

))

)

(err-protect (physical-time-steps 1

(rpgetvar 'max-iterations-per-step)))))

(begin

(remove-file run-switched/stop-filename)

(format "~%(run-switched ~d) Stopped~%" (car t-steps)))

(format "~%(run-switched ~d) Completed~%" (car t-steps))))

