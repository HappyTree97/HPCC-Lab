#include <mpi.h>
#include <stdio.h>
#include <math.h>
int main (int argc, char *argv[])
{
	int rank;
	int target;
	int nTasks;
	double sendArr[16384], recvArr[16384];
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nTasks);
	MPI_Status Stat;
	int i;
	for (i = 0; i < 16384; i++) sendArr[i] = 1.2;
	if (rank == 0)  target = nTasks - 1;
	if (rank == nTasks - 1) target = 0;
	double 	start_time,
			end_time,
			avg_time;
	avg_time = 0;
	if (rank == 0) {
		start_time = MPI_Wtime(); 
		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
		end_time = MPI_Wtime();
		avg_time = end_time - start_time + avg_time;

		start_time = MPI_Wtime(); 
		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
		end_time = MPI_Wtime();
		avg_time = end_time - start_time + avg_time;

		start_time = MPI_Wtime(); 
		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
		end_time = MPI_Wtime();
		avg_time = end_time - start_time + avg_time;

		start_time = MPI_Wtime(); 
		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
		end_time = MPI_Wtime();
		avg_time = end_time - start_time + avg_time;

		start_time = MPI_Wtime(); 
		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
		end_time = MPI_Wtime();
		avg_time = end_time - start_time + avg_time;

		avg_time = avg_time / 5;

		printf("Time: %f\n", avg_time);
		printf("BandWidth: %f kbit/s\n", 128*8 / avg_time);
	}
	else if (rank == nTasks - 1){
		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);

		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);

		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);

		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);

		MPI_Send(sendArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD);
		MPI_Recv(recvArr, 1, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &Stat);
	}
	MPI_Finalize();
}