#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#define MOD 10000007
#define MULTIPLIER 1234
#define INCREMENT 12345
long long seed;
#pragma omp threadprivate(seed)
long lcg()
{
	seed = (MULTIPLIER*seed + INCREMENT)%MOD;
	return seed;
}

void s_pseudo_rand(long p_seed)
{
	seed = p_seed;
}
double pseudo_rand()
{
	long rand_num = lcg();
	//printf("%ld\n", rand_num);
	return 2.0*rand_num/(MOD-1);
}

double pseudo_rand_parallel(int id,long seed)
{

}

double seq_pimonte(long num_steps,int r)
{
	double cx =1.0, cy = 1.0;
	double px,py,d;
	double count = 0;
	s_pseudo_rand(1);
	for (int i = 0; i < num_steps; ++i)
	{
		px = pseudo_rand();
		py = pseudo_rand();
		//printf("%lf %lf\n", px,py);
		d = sqrt((px - 1)*(px - 1) + (py - 1)*(py - 1));
		if(d <= r)
		{
			count++;
		}
	}

	return 4.0*count/num_steps;

}

double parallel_pimonte(long num_steps,int r,int NUM_THREADS)
{
	double cx =1.0, cy = 1.0;
	double px,py,d;
	double count = 0;
	omp_set_num_threads(NUM_THREADS);
	#pragma omp parallel private(px,py,d)
	{
		long id = omp_get_thread_num();

		s_pseudo_rand(id + 1);
		#pragma omp for reduction(+:count)
		for(int i=0;i<num_steps;++i)
		{
			px = pseudo_rand();
			py = pseudo_rand();
			d = sqrt((px - 1)*(px - 1) + (py - 1)*(py - 1));
			if(d <= r)
			{
				count = count + 1;
			}
		}
	}
	return 4.0*count/num_steps;
}

int main()
{
	long num_steps = 10000000;
	double time_taken_seq,time_taken_parallel;
	double pi;

	time_taken_seq = omp_get_wtime();
	pi = seq_pimonte(num_steps,1);
	time_taken_seq = omp_get_wtime() - time_taken_seq;
	printf("Sequential program : %lf \n", pi);

	int NUM_THREADS = 2;
	while(NUM_THREADS<=8)
	{
		time_taken_parallel = omp_get_wtime();
		pi = parallel_pimonte(num_steps,1,NUM_THREADS);
		time_taken_parallel = omp_get_wtime() - time_taken_parallel;
		printf("Parallel program : %lf with %d threads\n",pi,NUM_THREADS );
		printf("Speed up : %lf\n", time_taken_parallel/time_taken_seq);
		NUM_THREADS++;
	}

}