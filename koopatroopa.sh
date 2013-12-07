#!/bin/bash

for i in $(seq 1 $1);
do
	mpiexec -machinefile machine_file -np $2 main_exe	
done
