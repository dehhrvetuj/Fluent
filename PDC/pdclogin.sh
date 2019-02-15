#!/bin/bash

if [ $# = 1 ] ; then 
    kinit -l $1 --forwardable congwang@NADA.KTH.SE
fi 


klist -Tf

ssh -Y congwang@tegner.pdc.kth.se
