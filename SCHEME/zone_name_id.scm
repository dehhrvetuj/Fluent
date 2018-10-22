;; To get the ID of a Cell Zone or a Boundary Condition, use the command zone-name->id. 
;;  Examples: 
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
