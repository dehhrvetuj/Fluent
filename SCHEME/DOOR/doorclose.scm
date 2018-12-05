;
;
;
; SET TIMESTEP, DOOR OPEN STEP AND DOOR SLICE NUMBER
(define timestep 0.01)
(define doorstep 0.10)
(define doorslice 50)
;
;
; SET DOOR OPEN START TIME
(define dooropen 1)
(define doorclose 11)
;
;
; DEFINE RELATED VARIABLES
(define doortime 0)
(define flowtime 0)
(define doornum 0)
(define open #f)
(define close #f)
;
;
; GET FLOWTIME
(set! flowtime (%rpgetvar 'flow-time))
(display flowtime)
(display "****")
;
;
; GET DOORTIME
(set! doortime (round (/ (- flowtime doorclose) timestep)))
(display doortime)
(display "****")
;
;
;
; GET THE SLICE NUMBER OF DOOR 
(set! doornum 
	(+ 1
		(quotient 
			doortime (round (/ doorstep timestep))
		)
	)
)
(display doornum)
(display "****")
;
;
;
;
; TEST IF DOOR SHOULD CLOSE OR NOT
(set! close
	(if (and 			
			(>= flowtime (- doorclose 1e-6) )
			(<= doornum doorslice)
		)
	#t
	#f
	)
)
(set! close 
	(if (and
			close
			(= 
				(remainder doortime (round (/ doorstep timestep)))
				0
			)
		)	
	#t
	#f
	)
)
(display close)
(display "****")
;
;
;
(display (format #f "xdoor-d~02d\n" (- 51 doornum)))
;
;
;
; OPEN THE CORRESPONDING DOOR SLICE
(cond
	((eq? close #t)
		; (display (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall " (- 51 doornum)))
		(ti-menu-load-string 
			(format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall " (- 51 doornum))
		)
		(ti-menu-load-string "\n")
		
		(ti-menu-load-string 
			(format #f "/define/boundary-conditions/wall xdoor-d~02d , , , , yes heat-flux , , , , , , 0.90 yes trap , , , , , , , ," x)
		)	
		(ti-menu-load-string "\n")
		
		(ti-menu-load-string 
			(format #f "/define/boundary-conditions/wall xdoor-d~02d-shadow , , , , yes heat-flux , , , , , , 0.90 yes trap , , , , , , , ," x)
		)
		(ti-menu-load-string "\n")		
	)	
)
