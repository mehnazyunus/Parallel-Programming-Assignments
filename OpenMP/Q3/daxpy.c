#include <omp.h>
#include <stdio.h>
#define SIZE 65536

void seq_daxpy(double x[],double y[],double a)
{
	int i;
	for(i=0;i<SIZE;++i)
		x[i] = a*x[i] + y[i];
}

void parallel_daxpy(double x[],double y[],double a,int NUM_THREADS)
{
	int nthreads=0;
	omp_set_num_threads(NUM_THREADS);
	int i;
	#pragma omp parallel for private(i)
		for(i=0;i<SIZE;i++)
		{
			x[i] = a*x[i] + y[i];
		}

}
int main()
{
	double x[SIZE],y[SIZE],a=1;
	int i;
	//Filling the arrays with some values
	for(i=0;i<SIZE;++i)
		x[i] = y[i] = i+1;

	//Serial Execution
	double time_taken_parallel,time_taken_serial;
	time_taken_serial = omp_get_wtime();
	seq_daxpy(x,y,a);
	time_taken_serial = omp_get_wtime() - time_taken_serial;
	printf("Time taken for serial function : %lf ms\n", time_taken_serial*100);

	//Parallel Execution
	int NUM_THREADS = 2;
	while(NUM_THREADS<=20)
	{
		time_taken_parallel = omp_get_wtime();
		parallel_daxpy(x,y,a,NUM_THREADS);
		time_taken_parallel = omp_get_wtime() - time_taken_parallel;
		printf("Speed up : %lf  Threads : %d\n",time_taken_parallel/time_taken_serial,NUM_THREADS);
		
		NUM_THREADS++;
	}
}