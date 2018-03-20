#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define N 1000
#define SOURCE 0               /* rank of first task */
#define FROM_SOURCE 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main (int argc, char *argv[])
{
   int	numtasks,              
   	rank,                
   	numworkers,           
   	source,                
   	dest,                  
   	mtype,               
   	rows,                  
   	averow, extra, offset, 
   	i, j, k, rc;          
   double	a[N][N],        
         	b[N][N],           
         	c[N][N];           
   MPI_Status status;

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
   if (numtasks < 2 ) {
     printf("Can 3 processors tro len\n");
     MPI_Abort(MPI_COMM_WORLD, rc);
     exit(1);
     }
   numworkers = numtasks-1;

   if (rank == SOURCE)
   {
      for (i=0; i<N; i++)
         for (j=0; j<N; j++)
            a[i][j]= i+j;
      for (i=0; i<N; i++)
         for (j=0; j<N; j++)
            b[i][j]= i*j;

      double start = MPI_Wtime();

      averow = N/numworkers;
      extra = N%numworkers;
      offset = 0;
      mtype = FROM_SOURCE;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE, dest, mtype,
                   MPI_COMM_WORLD);
         MPI_Send(&b, N*N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         offset = offset + rows;
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, mtype, 
                  MPI_COMM_WORLD, &status);
      }

      
      double finish = MPI_Wtime();
      printf("Time: %f seconds.\n", finish - start);
   }


/**************************** worker task ************************************/
   if (rank > SOURCE)
   {
      mtype = FROM_SOURCE;
      MPI_Recv(&offset, 1, MPI_INT, SOURCE, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, SOURCE, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*N, MPI_DOUBLE, SOURCE, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, N*N, MPI_DOUBLE, SOURCE, mtype, MPI_COMM_WORLD, &status);

      for (k=0; k<N; k++)
         for (i=0; i<rows; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<N; j++)
               c[i][k] = c[i][k] + a[i][j] * b[j][k];
         }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, SOURCE, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, SOURCE, mtype, MPI_COMM_WORLD);
      MPI_Send(&c, rows*N, MPI_DOUBLE, SOURCE, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}