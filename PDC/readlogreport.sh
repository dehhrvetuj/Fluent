#!bin/bash

rexp='^[[:blank:]]+[[:digit:]]{1,6}[[:blank:]]+([[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+[[:blank:]]+)+[[:digit:]]{1,2}:[[:digit:]]{2}:[[:digit:]]{2}'

rexp='^[[:blank:]]+[[:digit:]]{1,6}[[:blank:]]+([[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+[[:blank:]]+)+'


gnuplot -persist <<-EOFMarker

#	set logscale y
	set title "Residuals"
	set ylabel 'Residual'
	set xlabel 'Iteration'
	set key outside
#    set yrange [20.95:21.04]
#    set xrange [80000:120000]
    set grid

	titles="1 2 3 4 5 6 7"

	plot for [col=$2:$3] "< cat $1 | grep -E  '$rexp' "  using 1:col title word(titles,col-$2+1) with lines

	pause 1
    
    exit

EOFMarker

