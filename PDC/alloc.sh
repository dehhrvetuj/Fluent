#!bin/bash

if [ $# = 1 ] ; 
then
    time=$1
else
    time='00:29:00'
fi


salloc -N 1 -t $time -A 2018-53

echo $SLURM_NODELIST  
