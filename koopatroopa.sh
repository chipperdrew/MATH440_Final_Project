#!/bin/bash
function power{
	for j in 'seq 1 $1';
	p=1
	do
		p=p*$2
	done
}



for i in 'seq 1 $1';
do
	p=power i 2
	
done