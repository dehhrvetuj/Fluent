#!/bin/bash


for ((i=0; i<100; i++)); do
	squeue | grep congwang
	sleep 1s
done
	