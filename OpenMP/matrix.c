#include <stdio.h>
#include <omp.h>
#define SIZE 1000

int a[SIZE][SIZE],b[SIZE][SIZE],c[SIZE][SIZE];

void seq_matmul(int a[][SIZE],int b[][SIZE])
{
	int i,j,k;

	for(i=0;i<SIZE;++i)
	{
		for(j=0;j<SIZE;++j)
		{
			c[i][j]=0;
			for(k=0;k<SIZE;++k)
			{
				c[i][j] = c[i][j] + a[i][k]*b[k][j];
			}
		}
	}
}

void parallel_matmul(int a[][SIZE],int b[][SIZE],int NUM_THREADS)
{

	int nthreads;
	int c[SIZE][SIZE];
	omp_set_num_threads(NUM_THREADS);
	int i,j,k;
	#pragma omp parallel for private(i)
		for(i=0;i<SIZE;i++)
		{
			#pragma omp parallel for private(j)
			for(j=0;j<SIZE;++j)
			{
				int t_sum = 0;
				#pragma omp parallel for reduction(+:t_sum) private(k)
				for(k=0;k<SIZE;++k)
				{
					t_sum = t_sum + a[i][k]*b[k][j];
				}

				#pragma omp critical
				{
					c[i][j] = t_sum;
				}
			}
		}
}
int main()
{
	int i,j;
	for(i=0;i<SIZE;++i)
		for(j=0;j<SIZE;++j)
			a[i][j]=b[i][j] = 1;

	double time_taken_serial = omp_get_wtime(),time_taken_parallel;
	seq_matmul(a,b);
	time_taken_serial = omp_get_wtime() - time_taken_serial;
	int NUM_THREADS=2;
	while(NUM_THREADS<=10)
	{
		time_taken_parallel = omp_get_wtime();
		parallel_matmul(a,b,NUM_THREADS);
		time_taken_parallel = omp_get_wtime() - time_taken_parallel;
		printf("Speed up for %d threads is %lf\n",NUM_THREADS,time_taken_parallel/time_taken_serial); 
		NUM_THREADS++;
	}
}	