#include<omp.h>
#include<stdio.h>

#define MAX_SIZE 65536
//#define NUM_THREADS 2

double X[MAX_SIZE], Y[MAX_SIZE], a;

int main(){
int i, NUM_THREADS=2;
double st,T1,Tn;
st = omp_get_wtime();
for(i=0;i<MAX_SIZE;i++)
X[i] = a*X[i] + Y[i];
T1 = omp_get_wtime() - st;

while(NUM_THREADS<8){
omp_set_num_threads(NUM_THREADS);
st = omp_get_wtime();
#pragma omp parallel
{
int i,id, nthrds;

id = omp_get_thread_num();
nthrds = omp_get_num_threads();
for(i=id;i<MAX_SIZE;i=i+nthrds)
X[i] = a*X[i] + Y[i];

}
Tn = omp_get_wtime() - st;
printf("No of threads: %d\nSpeedup: %f\n",NUM_THREADS, Tn/T1);
NUM_THREADS++;


}
}