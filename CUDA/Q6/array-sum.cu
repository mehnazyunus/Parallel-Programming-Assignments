#include <cuda.h>
#include <stdio.h>
#include <math.h>

__global__ void get_sum(float *dev_arr,float *dev_sum)
{
	int idx = blockIdx.x*blockDim.x + threadIdx.x;
	
	atomicAdd(dev_sum,dev_arr[idx]);
}
int main()
{
	float *dev_arr, *dev_sum , *arr,sum = 0;
	int N;
	
	printf("Enter Array size : \n");
	scanf(" %d",&N);

	arr = (float *)malloc(N*sizeof(float));
	//Filling the array
	for(int i=0;i<N;++i)
		arr[i] = 1.0;

	cudaMalloc((void **)&dev_arr,N*sizeof(float));
	cudaMalloc((void **)&dev_sum,sizeof(float));

	cudaMemcpy(dev_arr,arr,N*sizeof(float),cudaMemcpyHostToDevice);
	
	int size = ceil(sqrt(N));
	get_sum<<<size,size>>>(dev_arr,dev_sum);

	cudaMemcpy(&sum,&dev_sum,sizeof(float),cudaMemcpyDeviceToHost);
	printf("The sum of the array is %f\n",sum );

	cudaFree(dev_arr);
	cudaFree(dev_sum);
	cudaFree(dev_N);
	return 0;
}