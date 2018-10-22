;; To get the ID of a Cell Zone or a Boundary Condition, use the command zone-name->id. 
;; Examples: 
;; Example 1: display the ID of a zone in the TUI: 
(display (zone-name->id 'nameofthezone)) 
;; 

;; Example 2: store the ID of the zone in a variable: 
(rp-var-define 'int-zone 1 'integer #f) 
(rpsetvar 'int-zone (zone-name->id 'nameofthezone)) 
(rpgetvar 'int-zone) 
;; 

;; To get the ID of a post-processing surface, use the command surface-name->id:
;; Example: display the ID of a surface in the TUI: 
(display (surface-name->id 'nameofthesurface)) 
;; 

;; Before executing this command it is a good practice to check if the surface exists or not using the boolean command: 
(surface-name/id? 'nameofthesurface)



;; Get all zone id
(rp-var-define 'a () 'list #f)
(rpsetvar 'a ())
(for-each (lambda (t) (rpsetvar 'a (list-add 
									(rpgetvar 'a) (thread-id t))))
          (get-all-threads))
(rpgetvar 'a)

;; Get all zone names
(rp-var-define 'b () 'list #f)
(rpsetvar 'b ())
(for-each (lambda (t) (rpsetvar 'b (list-add
									(rpgetvar 'b) (thread-name t))))
          (get-all-threads))
(rpgetvar 'b)

