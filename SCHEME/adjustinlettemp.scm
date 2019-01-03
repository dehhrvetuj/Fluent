;
;
;
(define temp_tar 20.0)
(define temp_ave 0.00)
;
;
(if (not (rp-var-object 'inlet_temp))					; more decent way of defining RP variable
	(rp-var-define 'inlet_temp (- (pick-a-real "/report/surface-integrals area-weighted-avg  external_inlet () temperature , ") 273.15) 'double #f)
	()													; empty, do nothing
)
;
;
;
(set! temp_ave (- (pick-a-real "/report/surface-integrals area-weighted-avg point-1 point-2 () temperature , ") 273.15) )
(display temp_ave)
(display "****\n")
;
;
;
;
(if (> (abs (- temp_ave temp_tar)) 0.04)
	(begin
		(display "Adjust Inlet Temperature  ")
		(rpsetvar 'inlet_temp (+ (%rpgetvar 'inlet_temp) (* (- temp_tar temp_ave) 0.8)))
		(ti-menu-load-string
			(format #f "/define/boundary-conditions velocity-inlet external_inlet , , , , , , , , , ~3.3f , , , , , , , " (+ (%rpgetvar 'inlet_temp) 273.15))
		)
	)
    (display "Don't Adjust Inlet Temperature  ")
)
(display "\n****")
(display (%rpgetvar 'inlet_temp))
(display "****\n")

