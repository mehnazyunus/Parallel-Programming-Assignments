#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 4
int nthreads;
double seq_pi_calc(long num_steps)
{
	double sum=0.0,x;
	int i;
	for(i=0;i<num_steps;++i)
	{
		x = (i + 0.5)/num_steps;
		sum = sum + 4.0/(1.0 + x*x);
	}
	return sum/num_steps;
}
double parallel_pi_calc(long num_steps)
{
	double sum=0.0;
	omp_set_num_threads(NUM_THREADS);
	
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		int increment = omp_get_num_threads();
		if(ID == 0)
			nthreads = increment;
		long j;
		double t_sum=0,x;
		for(j=ID;j < num_steps;j+=increment)
		{
			x = (j + 0.5)/num_steps;
			t_sum = t_sum + 4.0/(1.0 + x*x);
		}

		#pragma omp critical
		{
			sum = sum + t_sum;
		} 
		
	}

	return (double)sum/num_steps;	
}
int main()
{
	int i;
	static long num_steps = 10000000;
	double time_taken,pi;
	//Sequential Pi Calculation
	time_taken = omp_get_wtime();
	pi = seq_pi_calc(num_steps);
	time_taken = omp_get_wtime() - time_taken;
	printf("Value of Pi by Sequential Calculation  : %lf\n",pi );
	printf("Time taken : %lf ms\n",time_taken*100);

	//Parallel Pi Calculation
	time_taken = omp_get_wtime();
	pi = parallel_pi_calc(num_steps);
	time_taken = omp_get_wtime()-time_taken;	
	printf("Value of Pi by Parallel Calculation  : %lf\n", pi);
	printf("Time taken by %d threads : %lf ms\n", nthreads,time_taken*100);
}	