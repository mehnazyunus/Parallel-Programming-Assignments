#include <stdio.h>
#include <mpi.h>
#define SIZE 10000000

int x[SIZE],y[SIZE];
int main(int argc, char const *argv[])
{
	int rank,size;
	int a=1;

	double time;
	for(int i=0;i<SIZE;++i)
	{
		x[i] = y[i] = 1;
	}

	MPI_Init(NULL,NULL);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	time = MPI_Wtime();
	for(int i = rank; i<SIZE; i+=size)
		x[i] = a*x[i] + y[i];
	
	time = MPI_Wtime() - time;
	printf("Time take by process %d %lf\n",rank,time );
	MPI_Finalize();

	return 0;
}