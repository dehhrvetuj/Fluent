#!bin/bash


gnuplot -persist <<-EOFMarker

	set logscale y
	set title "Residuals"
	set ylabel 'Residual'
	set xlabel 'Iteration'
	set key outside

	titles="iter continuity Ux Uy Uz energy k epsilon"

	plot for [col=2:8] "< cat $1 | grep -E  '^[[:blank:]]+[[:digit:]]{1,5}[[:blank:]]+([[:digit:]]\.[[:digit:]]+e[+-][[:digit:]]+[[:blank:]]+)+'"  using 1:col title word(titles,col) with lines

	
	pause 1

    exit
  
EOFMarker

