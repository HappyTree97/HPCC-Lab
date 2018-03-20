#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
	int rank;
	int nTasks;
	unsigned long pCount, pCountSum;
	double pi;
	int source = 0;
	long N = 100000000;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nTasks);

	srand((int)time(0));

	int i;
	pCount = 0;
	for (i = 0; i< N; i++) {
		double x = (double) rand() / (double) RAND_MAX;
		double y = (double) rand() / (double) RAND_MAX;
		if (x*x + y*y < 1.0) pCount++;
	}

	if (rank == source){
		pCountSum = pCount;
		for (i = 1; i < nTasks; i++) {
			long recv;
			MPI_Status stt;
			MPI_Recv(&recv, 1, MPI_LONG, i, 1, MPI_COMM_WORLD, &stt);
			pCountSum += recv;
		}
		pi = (double)pCountSum * 4 / ((double)nTasks * N) ;
		printf("pi = %f\n", pi);
	} else {
		MPI_Send(&pCount, 1, MPI_LONG, source, 1, MPI_COMM_WORLD);
	}

		MPI_Finalize();
}
