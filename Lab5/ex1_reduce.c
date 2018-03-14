#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    int numtasks, rank, size ,dest, source, rc, count, tag = 1;
    
    MPI_Status Stat; // required variable for receive routines
    int local, global;

    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local = rank+1;
    MPI_Reduce(&local,&global,1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);    

    if(rank ==0)
        printf("Sum is : %d", global);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
