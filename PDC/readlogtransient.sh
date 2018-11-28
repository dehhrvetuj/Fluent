#!bin/bash

rexp='^[[:blank:]]+[[:digit:]]{1,5}[[:blank:]]+([+-]?[[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+[[:blank:]]+)+[+-]?[[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+$'

#grep -E  '$rexp' $1  > data.dat

gnuplot -persist <<-EOFMarker

#	set logscale y
	set title "Report"
	set ylabel 'Value'
#	set xlabel 'Time Step'
    set xlabel 'Flow Time'
	set key outside
#    set yrange [-1:0.5]
#   set xrange [4779:5100]
    set grid

	titles="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20"

	plot for [col=$2:$3] "< cat $1 | grep -E  '$rexp' "  using 2:col title word(titles,col) with lines

#    plot for [col=3:5] "< cat $1 | grep -E  '$rexp' " using 2:col title word(titles,col) with lines
	
	pause 1
    
    exit

EOFMarker
  

