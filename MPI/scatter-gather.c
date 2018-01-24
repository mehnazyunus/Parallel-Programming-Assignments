#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#define N 10

float arr[N+1],recvBuf[N+1];
int main(int argc, char const *argv[])
{
	int rank,world_size,namelen;
	char name[30];

	for(int i=0;i<N;++i)
		arr[i] = i + 1;

	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD,&world_size);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	MPI_Get_processor_name(name,&namelen);

	int *sendcounts = malloc(sizeof(int)*world_size);
	int *displs = malloc(sizeof(int)*world_size);
	

		int rem = N,displacement=0;
		for(int i=0;i<world_size;++i)
		{
			//printf("Hello\n");
			if(i == world_size - 1)
			{
				sendcounts[i] = rem;
			}
			else
			{
				sendcounts[i] = i + 1;
				rem-=sendcounts[i];
			}
			
			displs[i] = displacement;
			displacement += sendcounts[i];
		}

		MPI_Scatterv(arr,sendcounts,displs,MPI_FLOAT,recvBuf,N,MPI_FLOAT,0,MPI_COMM_WORLD);

		
		for(int i=0;i<N;++i)
		{	
			recvBuf[i] = sqrt(recvBuf[i]);
			printf(" %d %d %f\n",rank,i,recvBuf[i] );
		}

		MPI_Barrier(MPI_COMM_WORLD);

		MPI_Gatherv(recvBuf,N,MPI_FLOAT,arr,sendcounts,displs,MPI_FLOAT,0,MPI_COMM_WORLD);
	
		if(rank == 0)
		{
			for(int i=0;i<10;++i)
				printf("%f\n",recvBuf[i]);
		}
	MPI_Finalize();

	return 0;
}