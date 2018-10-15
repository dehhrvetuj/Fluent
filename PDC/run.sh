#!/bin/bash 

# The name of the script is myjob
#SBATCH -J congwang

# Only 1 hour wall-clock time will be given to this job
#SBATCH -t 1:00:00

# set the project to be charged for this
# should normally be of the format 2016-1 or 2016-16-1 or similar
#SBATCH -A 2018-53

# Number of nodes
#SBATCH -N 1

# Number of MPI processes per node 
#SBATCH --ntasks-per-node=24

#SBATCH -e error_file.e
#SBATCH -o output_file.o
#SBATCH --mail-type=ALL


# load module fluent v18.2
module add fluent/19.2 

# The Journal file
JOURNALFILE=journal.jou

# Total number of Processors
#NPROCS=24
NTASKS=`echo $SLURM_TASKS_PER_NODE | cut -c1-2`
NPROCS=`expr $SLURM_NNODES \* $NTASKS`

if [ $SLURM_NNODES -eq 1 ]; then
    # Single node with shared memory
    fluent 3ddp -g -t $NPROCS -i $JOURNALFILE > log.log 
else
    # Multi-node
    fluent 3ddp -g -slurm -t $NPROCS -mpi=pcmpi -pib -i $JOURNALFILE > log.log 
fi