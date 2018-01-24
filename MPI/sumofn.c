#include <stdio.h>
#include <mpi.h>
#define N 10

long long arr[N],sum[N];
int main(int argc, char const *argv[])
{
	int rank,world_size,namelen;
	char name[30];

	for(int i=0;i<N;++i)
		arr[N] = i+1;

	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD,&world_size);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	for(int i = rank;i<N;i+=world_size)
		sum[rank]+=arr[i];

	int mid_point;
	long long send_sum,recv_sum;
	mid_point = world_size - 1;
	do
	{
		
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Status status;
		if(mid_point % 2 == 0 && rank == 0)
		{

			MPI_Recv(&recv_sum,1,MPI_LONG_LONG_INT,rank + mid_point - 1,0,MPI_COMM_WORLD,&status);
			printf("M %d %d Sum: %lld \n",rank,rank + mid_point,recv_sum);	
			
			sum[0] += recv_sum;
			mid_point = mid_point/2;
		}
		else
		{
			mid_point = mid_point/2;
			
			if(rank <= mid_point)
			{	
				MPI_Recv(&recv_sum,1,MPI_LONG_LONG_INT,rank + mid_point + 1,0,MPI_COMM_WORLD,&status);
				sum[rank] += recv_sum;
				printf("Recv %d %d \n",rank,rank + mid_point + 1);
			}
			else
			{
				send_sum = sum[rank];
				MPI_Send(&send_sum,1,MPI_LONG_LONG_INT,rank - mid_point - 1 ,0,MPI_COMM_WORLD);
				printf(" Send %d %d \n",rank,rank - mid_point - 1);

			}
			
		}

		
		
	}while(mid_point > 1);

	printf("Over : %d\n",rank);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Barrier crossed\n");
	if(rank == 0)
	{
		printf("Sum of the array is %lld\n",sum[rank]);
	}
	
	MPI_Finalize();

	return 0;
}