; 1st APPROACH
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


; 2nd APPROACH
; To use one SCHEME command (pick-a-real "/rep/surf/facet-max (wall_example) pressure n") 
; to get maximum pressure of a face zone called 'wall_example'.

(define limit 100000)

(if (not (rp-var-object 'explosion_time))					; more decent way of defining RP variable
	(rp-var-define 'explosion_time -999.9 'double #f)
	()														; empty, do nothing
)															

(if (< (%rpgetvar 'explosion_time) 0.0)
	(if (> (pick-a-real "/rep/surf/facet-max (wall_example) pressure n") limit)
    	(begin
      		(display "Limit Pressure Reached.  ")
      		(rpsetvar 'explosion_time (%rpgetvar 'flow-time))

      		(ti-menu-load-string "/def/bc/mod-zones/zone-type wall_example pressure-inlet")
      		(ti-menu-load-string "/def/bc/pressure-inlet wall_example , , 300000 , , , 300 , , , , , , , , , , , , , ,")
      	)
    	(display "Limit Pressure Not Yet Reached.  ")
	)
)

; ******************** IMPORTANT TO NOTE *********************************
; ++ You can put face zone numbers into the /report/surface command... BUT, whatever numbers are used there, 
; 	 they are not the same numbers as zone ID numbers. (Yeah, I was surprised, too.) So I have avoided the zone ID altogether and used the name only.
; ++ You could put that longer set of Scheme commands into a file called check_pres.scm, and then run it with a TUI command 
;    /file/read-macro check_pres.scm. I cunningly made sure that the RP-variable is not re-defined every time.
; ++ You can put the above TUI command in Calculation Activities//Execute Commands, to run at a specified frequency. 
; https://www.cfd-online.com/Forums/fluent-udf/153731-possible-changing-bc-wall-pressure-outlet-udf-trigger.html


