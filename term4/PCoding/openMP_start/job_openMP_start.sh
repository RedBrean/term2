#!/bin/bash
#PBS -l walltime=ype,nodes=1:ppn=4
#PBS -N lesha_job_openMP_start
#PBS -q batch

# Перемещение объектных файлов в output
mv ~/openMP_start/lesha_job_*.o* ~/openMP_start/output_history/ 2>/dev/null
mv ~/openMP_start/lesha_job_*.e* ~/openMP_start/output_history/ 2>/dev/null

cd ~/openMP_start

./openMP_start.out
