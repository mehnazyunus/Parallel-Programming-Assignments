#include <stdio.h>
#include <mpi.h>

struct dd
{
	char c;
	int a[2];
	float f[4];
};

int main(int argc, char const *argv[])
{
	int rank,world_size,namelen;
	char name[30];

	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD,&world_size);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	MPI_Get_processor_name(name,&namelen);


	char c;
	int a[2];
	float f[4];

	
	
	MPI_Finalize();

	return 0;
}