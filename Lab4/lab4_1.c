#include <mpi.h>
#include <stdio.h>
#include <math.h>

int isprime(int n) {
int i,squareroot;
if (n>10) {
   squareroot = (int) sqrt((double) n);
   for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
         return 0;
   return 1;
   }
else
	if (n == 2 || n == 3 || n == 5 || n ==7 ) return 1;
    else
    	return 0;
}

int main (int argc, char *argv[])
{
	int rank;
	int nTasks;
	long pCount, pCountSum;
	long limit = 1000000000;
	int source = 0;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nTasks);

	long step = nTasks*2;
	long start = rank*2 + 1;
	long i;
	pCount = 0;
	double start_time = MPI_Wtime(); 
	if (rank == source){
		for (i = start; i <= limit; i+=step){
			if (isprime(i) == 1) {
				pCount++;
			}
		}
		MPI_Reduce(&pCount,&pCountSum,1,MPI_INT,MPI_SUM,source,MPI_COMM_WORLD);
		double end_time=MPI_Wtime();
		printf("So prime number tu 1 -> 1000000000 la: %d\n", pCountSum + 1); // bo qua so chan nen chua tinh 2.
		printf("Mat thoi gian: %.2lf\n", end_time - start_time); 
	} else {
		for (i = start; i <= limit; i+=step){
			if (isprime(i) == 1) {
				pCount++;
			}
		}
		MPI_Reduce(&pCount,&pCountSum,1,MPI_INT,MPI_SUM,source,MPI_COMM_WORLD);
	}
	MPI_Finalize();
}