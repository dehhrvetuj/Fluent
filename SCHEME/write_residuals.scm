; Sometimes there is need to write the residuals to a separate text file. Normally, the residuals are printed in the FLUENT console and they are also written to the data file. However, in some situations, users want to write the residuals to a file at the end of each iteration.
; The following steps can be used to write the solution residuals to a text file.
; 1. Read the case/data files in FLUENT.
; 2. Read the Scheme file (init.scm) using the File-->Read-->Scheme command. The content of the Scheme file should be as follows (please copy the content between the dashed line into a text file and save it as 'init.scm'):
--------------------------------------------------------------
(define port)
(set! port (open-output-file "residuals.dat"))

(do
((i 0 (+ i 1)))
((= i (length (solver-residuals))))
(format port "~a ~2t" (car (list-ref (solver-residuals) i)))
)
(newline port)
---------------------------------------------------------------

; 3. Set the following execute command at every iteration using Solve --> Execute Commands:
; file read-journal residual.jou
; The content of the journal file (residual.jou) should be as follows (please copy the content between the dashed line into a text file and save it as 'residual.jou'):

---------------------------------------------------------------
(do
((i 0 (+ i 1)))
((= i (length (solver-residuals))))
(format port "~a ~2t" (cdr (list-ref (solver-residuals) i)))
)
(newline port)
---------------------------------------------------------------

; 4. Now compute the solution using the Solve->Iterate command. This will write out a text file ('residuals.dat') containing the solution residual history.