#!bin/bash

if [ $# = 2 ] ; 
then
    num=$2
else
    num=8
fi


# suitable for both steady and transient
rexp='^[[:blank:]]+[[:digit:]]{1,5}[[:blank:]]+(-*[[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+[[:blank:]]+)+[[:digit:]]{1,2}:[[:digit:]]{2}:[[:digit:]]{2}'

# suitable for only steady case
#rexp='^[[:blank:]]+[[:digit:]]{1,6}[[:blank:]]+([[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+[[:blank:]]+)+'

cat $1 | grep -E  '$rexp' 

gnuplot -persist <<-EOFMarker

	set logscale y
	set title "Residuals"
	set ylabel 'Residual'
	set xlabel 'Iteration'
	set key outside
#    set yrange [1E-8:1]
#    set xrange [4779:5100]
    set grid

	titles="iter continuity Ux Uy Uz energy k epsilon do-I"

	plot for [col=2:$num] "< cat $1 | grep -E  '$rexp' "  using 1:col title word(titles,col) with lines

	pause 1
    
    exit

EOFMarker
 
