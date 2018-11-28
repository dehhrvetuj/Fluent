#!bin/bash

# find a string folloing 'number tracked ='
rexp='(?<=number tracked =\s)\w+' 

grep -oP '(?<=number tracked =\s)\w+' $1 > $2

gnuplot -persist <<-EOFMarker


#   set logscale y
    set grid
    
#   Note grep -oP rather than grep -E
#    plot "< cat $1 | grep -oP '(?<=number tracked =\s)\w+' "
    plot "< cat $2 "
  
  
    pause 1
                                  
    exit

EOFMarker
