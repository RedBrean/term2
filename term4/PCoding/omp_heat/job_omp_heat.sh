#!/bin/bash
#PBS -l walltime=00:01:00,nodes=1:ppn=4
#PBS -N lesha_job_omp_heat
#PBS -q batch

# Перемещение объектных файлов в output
mv ~/omp_heat/lesha_job_*.o* ~/omp_heat/output_history/ 2>/dev/null
mv ~/omp_heat/lesha_job_*.e* ~/omp_heat/output_history/ 2>/dev/null

cd ~/omp_heat

./omp_heat.out 1000
