#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define _128KB 16384
#define _256KB 32768
#define _512KB 65536
#define _1024KB 131072
int main(int argc, char *argv[])
{

	int rank;
	int target;
	int nTasks;
	int numTest = 5;
	double sendArr[_128KB], recvArr[_128KB];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nTasks);
	MPI_Status Stat;
	int i;
	for (i = 0; i < _128KB; i++)
		sendArr[i] = 1.2;
	if (rank == 0)
		target = nTasks - 1;
	if (rank == nTasks - 1)
		target = 0;
	double start_time,
		end_time,
		avg_time;
	avg_time = 0;
	if (rank == 0)
	{
		for (i = 0; i < numTest; i++)
		{
			start_time = MPI_Wtime();
			MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
			MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
			end_time = MPI_Wtime();
			avg_time = end_time - start_time + avg_time;
		}

		avg_time = avg_time / 5;

		printf("Time: %f\n", avg_time);
		printf("BandWidth: %f kbit/s\n", 128 * 8 / avg_time);
	}
	else if (rank == nTasks - 1)
	{

		for (i = 0; i < numTest; i++)
		{
			MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
			MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
		}
	}
	MPI_Finalize();
}