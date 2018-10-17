; 1st implementation
; Change the boundary conditon from 'wall' to 'pressure-outlet' if the maximum pressure exceeds the given limit.
; The maximum pressure value is obtained from UDF

(rp-var-define 'pressure_on_wall 0.0 'real #f)
(rp-var-define 'explosion_time 0.0 'float #f)
(define exploded #f)
(define limit 100000)
(define ts 50)


(do ((i 1 (+ 1 i) ))((> i ts))


	(ti-menu-load-string "solve dti 1 10")

	(cond((eq? exploded #f)
		(%udf-on-demand 'pres_check);### CHECK UDF NAME!!! ###

		(display "Limit Pressure Not Yet Reached")
	
		(cond((> (%rpgetvar 'pressure_on_wall) limit)
			(display "Limit Pressure Reached")
			(set! exploded #t)
			(rpsetvar 'explosion_time (%rpgetvar 'flow-time))
			
			(ti-menu-load-string "def bc mz zt 6 pi");### CHECK ZONE ID ###
			(ti-menu-load-string "def bc pi 6 , , 300000 , , , , , , , , ,");### CHECK ZONE ID ###
		))
	))

)
