#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    int numtasks, rank, size, dest, source, rc, count, tag = 1;

    MPI_Status Stat; // required variable for receive routines
    int n = 30;
    int a;
    int **sendRoot;
    int *recvRoot;
    int *recvClient;
    int i = 0,j=0;
    int sendRootCount = 0, recvRootCount = 0;
    int sendClientCount = 0, recvClientCount = 0;
    int global = 0, local = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    sendRootCount = size;

    if (n % size == 0)
        recvRootCount = n / size;
    else
        recvRootCount = n / size + 1;
    
    if (rank == 0)
    {
        sendRoot = malloc(sendRootCount * sizeof(int*));
        for (i = 0; i<sendRootCount; i++)
            sendRoot[i] = malloc(recvRootCount* sizeof(int));

        for (i=0; i<sendRootCount; i++){
            for(j=0; j<recvRootCount;j++){
                if(i*sendRootCount+j<n)
                    sendRoot[i][j] = i*sendRootCount+j;
                else 
                    sendRoot[i][j] =0;
            }
        }
    }

    MPI_Scatter(&sendRoot, sendRootCount, MPI_INT, &recvRoot, recvRootCount, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < recvRootCount; i++)
    {
        local += recvRoot[i];
    }

    printf("Local sum is %d ", local);

    MPI_Gather(&local, 1, MPI_INT, &recvClient, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf(sendRootCount);
        for (i = 0; i < sendRootCount; i++)
        {
            global += recvClient[i];
        }
        printf("Global sum is %d", global);
    }

    MPI_Finalize();
    return 0;
}
