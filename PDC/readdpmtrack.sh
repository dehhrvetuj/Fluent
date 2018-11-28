grep -E '^number tracked =[[:blank:]]+[[:digit:]]{2,9}' unsteady.log

echo 'hello world'|grep -oP '(?<=number tracked =\s)\w+'   #是找出以hello为前缀之后的字符串


gnuplot -persist <<-EOFMarker


    set logscale y
    set grid
  
    plot for [col=1] "< cat $1 | grep -E '$rexp' "
  
  
  pause 1
                                  
  exit
