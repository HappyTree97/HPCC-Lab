#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    int numtasks, rank, size, dest, source, rc, count, tag = 1;

    MPI_Status Stat; // required variable for receive routines
    int n = 25;
    int a;
    int * sendRoot;
    int * recvRoot;
    int * recvClient;
    int i = 0;
    int sendRootCount = 0, recvRootCount = 0;
    int sendClientCount = 0, recvClientCount = 0;
    int global = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    

    if (n % size == 0)
        recvRootCount = n / size;
    else
        recvRootCount = n / size + 1;
    sendRootCount = recvRootCount;
    if (rank == 0)
    {
	
        int t = sendRootCount * size;
        sendRoot = malloc(t * sizeof(int));
        for (i = 0; i < t; i++)
        {
	    
            if (i < n)
                sendRoot[i] = i;
            else
                sendRoot[i] = 0;
        }
    
    printf("send buffer size :%d, recv buffer size : %d\n",sendRootCount,recvRootCount);
    
    }
    recvRoot = malloc(sizeof(int)*recvRootCount);
    MPI_Scatter(sendRoot, sendRootCount, MPI_INT, recvRoot, recvRootCount, MPI_INT, 0, MPI_COMM_WORLD);
    int local =0;
    //if(i==1){
   
 
    	for (i = 0; i < recvRootCount; i++)
        {
            printf("Process [%d] recieved %d\n",rank,recvRoot[i]);
            local += recvRoot[i];
    	}
    //}
    printf("Local sum is %d ", local);
    if(rank ==0){
    	recvClient = malloc(size*sizeof(int));
    }
    MPI_Gather(&local, 1, MPI_INT, recvClient, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (i = 0; i < size; i++)
        {
            global += recvClient[i];
        }
        printf("Global sum is %d", global);
    }

    MPI_Finalize();
    return 0;
}


