grep -E '^number tracked =[[:blank:]]+[[:digit:]]{2,9}' unsteady.log

echo 'hello world'|grep -oP '(?<=hello\s)\w+'   #是找出以hello为前缀之后的字符串
