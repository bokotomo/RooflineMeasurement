#!/bin/sh
#PBS -q u-short
#PBS -l select=1:mpiprocs=18:ompthreads=18
#PBS -W group_list=gc20f
#PBS -l walltime=02:00:00
cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh
module load intel/16.0.3.210
numactl -l ./a.out jobtest 18 > ./output.log 2>&1

