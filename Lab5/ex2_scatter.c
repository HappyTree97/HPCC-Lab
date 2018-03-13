#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    int numtasks, rank, size ,dest, source, rc, count, tag = 1;
    
    MPI_Status Stat; // required variable for receive routines
    int n = 10;
    int a;
    int * sendRoot ;
    int * recvRoot;
    int sendClient;
    int * recvClient;

    int sendRootCount=0, revRootCount=0;
    int sendClientCount =0; recvClientCount =0;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank =0){
        sendRootCount=n/size;
        
        if(n%size==0){
            recvRootCount= size;
            sendRoot = malloc(n*sizeof(int))
            for(int i=0; i<n;i++){
                sendRoot[i] = i;
            }
        }
        else{
            recvRootCount= size+1;
            int t =(size+1)*sendRootCount;
            sendRoot = malloc(t*sizeof(int));
            for(int i=0; i<t; i++){
                if(i<n)
                    sendRoot[i]=i;
                else 
                    sendRoot[i] =0;
            }
        }
    }
    sendClientCount = 1;
    recvClientCount = n/size;
    MPI_Scatter(&sendRoot, sendRootCount, MPI_INT, &recvRoot, recvRootCount, MPI_INT, 0, MPI_COMM_WORLD)
    
    
    int local =0;
    for(int i =0; i<recvClientCount; i++){
        local += recvClient[i];
    }

    MPI_Gather(&sendClient, 1, MPI_INT, &recvClient, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank ==0){
        int global =0;
        for(int i=0;i<sendRootCount;i++){
            global += recvRoot(i);
        }
        printf("Global sum is %d", global)
    }

    MPI_Finalize();
    return 0;
}