;
; SET DOOR FROM INTERIOR TYPE TO  WALL TYPE
(do ((x 1 (+ x 1))) ((> x 50))
	(ti-menu-load-string 
		(format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall" x)
	)
	(ti-menu-load-string "")
)
;
;
;
;
(define doortime 0)
(define flowtime 0)
(define dooropen 360)
(define timestep 0.01)
(define doorstep 0.1)
(define doornum 0)
(define open #f)
;
; GET FLOWTIME
;(set! flowtime (%rpgetvar 'flow-time))
(set! flowtime 360.199999999)
(display flowtime)
;
; GET DOORTIME
(set! doortime (round (/ (- flowtime dooropen) timestep)))
(display doortime)
;
; TEST IF DOOR SHOULD OPEN OR NOT
(set! open 
	(if (= 
			(remainder doortime (round (/ doorstep timestep)))
			0
		)
		#t
		#f
	)
)
(display open)
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
;
;
;
;
;
(cond((eq? open #t)	
		; (ti-menu-load-string 
			; (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall" doornum)
		; )
		(display (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall" doornum))
	)	
)
	

/define/boundary-conditions/wall xdoor-d01 , , , , yes heat-flux , , , , , , 0.88 yes trap , , , , , ,
/define/boundary-conditions/wall xdoor-d01-shadow , , , , yes heat-flux , , , , , , 0.88 yes trap , , , , , ,




