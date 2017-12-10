gnuplot -persist <<-EOFMarker

#	set logscale y
	set title "Report"
	set ylabel 'Value'
	set xlabel 'Time Step'
	set key outside
#   set yrange [1E-8:1]
#   set xrange [4779:5100]
  set grid

	titles="time-step flow-time report1 report2 report3"

#	plot for [col=2:8] "< cat $1 | grep -E  '$rexp' "  using 1:col title word(titles,col) with lines

    plot for [col=3:5] "< cat $1 | grep -E  '$rexp' " using 1:col title word(titles,col) with lines
	
	pause 1
    
    exit

EOFMarker
