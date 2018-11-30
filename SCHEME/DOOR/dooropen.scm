;
; (quotient 21 10) = 2; (remainder 20 10)= 0 
;
; SET TIMESTEP, DOOR OPEN STEP AND DOOR SLICE NUMBER
(define timestep 0.01)
(define doorstep 0.10)
(define doorslice 50)
;
;
; SET DOOR OPEN START TIME
(define dooropen 2521)
(define doorclose 2531)
;
;
; DEFINE RELATED VARIABLES
(define doortime 0)
(define flowtime 0)
(define doornum 0)
(define open #f)
;
;
; GET FLOWTIME
(set! flowtime (%rpgetvar 'flow-time))
; (set! flowtime 2520.9999999)
(display flowtime)
(display "****")
;
;
; GET DOORTIME
(set! doortime (round (/ (- flowtime dooropen) timestep)))
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
; TEST IF DOOR SHOULD OPEN OR NOT
; flow time >= door open time && door number <= total door slice number
(set! open
	(if (and 			
			(>= flowtime (- dooropen 1e-6))
			(<= doornum doorslice)
		)
	#t
	#f
	)
)
; doortime%10 == 0, i.e. open at 0.1 0.2 0.3 etc.
(set! open 
	(if (and
			open
			(= 
				(remainder doortime (round (/ doorstep timestep)))
				0
			)
		)	
	#t
	#f
	)
)
(display open)
(display "****")
;
;
;
(display (format #f "xdoor-d~02d\n" doornum))
;
;
;
; OPEN THE CORRESPONDING DOOR SLICE
(cond
	((eq? open #t)
		; (display 
			; (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d interior" doornum)
		; )
		(ti-menu-load-string 
			(format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d interior " doornum)
		)
	)	
)