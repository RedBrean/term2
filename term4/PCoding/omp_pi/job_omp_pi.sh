#!/bin/bash
#PBS -l walltime=00:01:00,nodes=1:ppn=4
#PBS -N lesha_job_omp_pi
#PBS -q batch

# Перемещение объектных файлов в output
mv ~/omp_pi/lesha_job_*.o* ~/omp_pi/output_history/ 2>/dev/null
mv ~/omp_pi/lesha_job_*.e* ~/omp_pi/output_history/ 2>/dev/null

cd ~/omp_pi

./omp_pi_for.out
