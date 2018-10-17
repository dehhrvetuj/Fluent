; 1st implementation
; Change the boundary conditon from 'wall' to 'pressure-outlet' if the maximum pressure on the wall exceeds the given limit.
; The maximum pressure value is obtained from UDF

(rp-var-define 'pressure_on_wall 0.0 'real #f)				; define RP variable
(rp-var-define 'explosion_time 0.0 'float #f)
(define exploded #f)										; define boolean variable
(define limit 100000)										; define real variable
(define ts 50)


(do ((i 1 (+ 1 i) ))((> i ts))								; do loop


	(ti-menu-load-string "solve dti 1 10")					; call FLUENT TUI through 'ti-menu-load-string'

	(cond((eq? exploded #f)									; condition: exploded == #f ?
		(%udf-on-demand 'pres_check);						; ### CHECK UDF NAME!!! ###

		(display "Limit Pressure Not Yet Reached")
	
		; make sure to include % before rpgetvar
		(cond((> (%rpgetvar 'pressure_on_wall) limit)		; condition: pressure_on_wall > limit ? 
			(display "Limit Pressure Reached")
			(set! exploded #t)
			(rpsetvar 'explosion_time (%rpgetvar 'flow-time))
			
			(ti-menu-load-string "def bc mz zt 6 pi")								; ### CHECK ZONE ID ###
			(ti-menu-load-string "def bc pi 6 , , 300000 , , , , , , , , ,")		; ### CHECK ZONE ID ###
		))
	))

)
