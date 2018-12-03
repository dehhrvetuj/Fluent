;
; SET DOOR FROM INTERIOR TYPE TO WALL TYPE
; (do ((x 1 (+ x 1))) ((> x 50))
	; (ti-menu-load-string 
		; (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall" x)
	; )
; )
;
;
; SET DOOR FROM INTERIOR TYPE TO FAN TYPE
; (do ((x 1 (+ x 1))) ((> x 50))
	; (ti-menu-load-string 
		; (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d fan" x)
	; )
; )
;
;
;
; SET DOOR TO WALL AND CHANGE CORRESPONDING WALL BOUNDARY CONDITIONS
(do ((x 1 (+ x 1))) ((> x 50))
	; (ti-menu-load-string 
		; (format #f "/define/boundary-conditions/modify-zones/zone-type xdoor-d~02d wall" x)
	; )
	(ti-menu-load-string 
		(format #f "/define/boundary-conditions/wall xdoor-d~02d , , , , yes heat-flux , , , , , , 0.95 yes trap , , , , , ," x)
	)	
	(ti-menu-load-string "\n")
;	
	(ti-menu-load-string 
		(format #f "/define/boundary-conditions/wall xdoor-d~02d-shadow , , , , yes heat-flux , , , , , , 0.95 yes trap , , , , , ," x)
	)
	(ti-menu-load-string "\n")	
)

; /define/boundary-conditions/wall xdoor-d01 , , , , yes heat-flux , , , , , , 0.88 yes trap , , , , , ,					; SET WALL EMISSIVITY TO 0.88
; /define/boundary-conditions/wall xdoor-d01-shadow , , , , yes heat-flux , , , , , , 0.88 yes trap , , , , , ,				; SET WALL-shadow EMISSIVITY TO 0.88
; /define/models/dpm/injections set-injection-properties injection-0 , , , , , , , , , , , , , , , , , , , , 3600 , , , ,	; SET INJECTION STOP TIME TO 3600s 
