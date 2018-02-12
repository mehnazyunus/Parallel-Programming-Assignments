#include "wb.h"
#include <iostream>
#include <cuda.h>
using namespace std;
#define NUM_BINS 4096


__global__ void hist_kernel(unsigned int *deviceInput, unsigned int *deviceBins,unsigned int inputLength)
{
  int i = threadIdx.x + blockDim.x*blockIdx.x;
  
 if(i<inputLength)
    {int item = deviceInput[i];  atomicAdd(&(deviceBins[item]),1);}
 
}

__global__ void bins_cap(unsigned int *deviceBins)
{
  int i = threadIdx.x + blockDim.x*blockIdx.x;
  deviceBins[i] = min(127,deviceBins[i]);
}

int main(int argc, char *argv[]) {

 unsigned  int inputLength;
  unsigned int *hostInput;
  unsigned int *hostBins;
  unsigned int *deviceInput;
  unsigned int *deviceBins;

 
  wbArg_t args = wbArg_read(argc,argv);
  FILE* inp = fopen(argv[1],"r");
	fscanf(inp,"%d",&inputLength);

  wbTime_start(Generic, "Importing data and creating memory on host");
	
  hostInput = new unsigned int[inputLength];
  
  for(int i=0;i<inputLength;i++)
  {
    fscanf(inp,"%d",&hostInput[i]);
  }
  
  hostBins = (unsigned int *)malloc(NUM_BINS * sizeof(int));
  
  wbTime_stop(Generic, "Importing data and creating memory on host");

  wbLog(TRACE, "The input length is ", inputLength);
  wbLog(TRACE, "The number of bins is ", NUM_BINS);

  wbTime_start(GPU, "Allocating GPU memory.");
  
  cout<<inputLength<<endl;
  int size = inputLength*sizeof(int);

  cudaMalloc((void **)&deviceInput,size);
  cudaMalloc((void **)&deviceBins,NUM_BINS*sizeof(unsigned int));
  cudaMemset(deviceBins,0,NUM_BINS*sizeof(unsigned int));
  CUDA_CHECK(cudaDeviceSynchronize());

  wbTime_stop(GPU, "Allocating GPU memory.");
  wbTime_start(GPU, "Copying input memory to the GPU.");

  cudaMemcpy(deviceInput,hostInput,size,cudaMemcpyHostToDevice);
  
  CUDA_CHECK(cudaDeviceSynchronize());
  wbTime_stop(GPU, "Copying input memory to the GPU.");

  wbLog(TRACE, "Launching kernel");
  wbTime_start(Compute, "Performing CUDA computation");
  
  int threads = 1024;
  hist_kernel<<<(inputLength-1)/1024+1, threads>>>(deviceInput,deviceBins,inputLength);
  bins_cap<<<4,1024>>>(deviceBins);
  wbTime_stop(Compute, "Performing CUDA computation");

  wbTime_start(Copy, "Copying output memory to the CPU");
  
  cudaMemcpy(hostBins,deviceBins,NUM_BINS*sizeof(unsigned int),cudaMemcpyDeviceToHost);
  CUDA_CHECK(cudaDeviceSynchronize());

  wbTime_stop(Copy, "Copying output memory to the CPU");

  wbTime_start(GPU, "Freeing GPU Memory");

  cudaFree(deviceInput);
  cudaFree(deviceBins);
  wbTime_stop(GPU, "Freeing GPU Memory");

  wbSolution(args, hostBins, NUM_BINS);
  free(hostBins);
  free(hostInput);
  return 0;
}
