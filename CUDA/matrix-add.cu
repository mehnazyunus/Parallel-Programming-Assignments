#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TILE_SIZE 16

void printMatrix(int *mat,int nrow,int ncol)
{
	for(int i=0;i<nrow;i++)
	{
		for(int j=0;j<ncol;j++)
			printf("%d ",mat[i*ncol+j]);
		printf("\n");
	}
}
__global__ void add_matrix(int *dev_mat1,int *dev_mat2,int *dev_sum_mat,int nrow,int ncol)
{
	//printf("Hello\n");
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;

	int idx = x + y*ncol;
	printf("Dimensions %d %d\n",nrow,ncol );
	if(x < ncol && y < nrow )
		dev_sum_mat[idx] = dev_mat1[idx] + dev_mat2[idx];
}

int main()
{
	int nrow=10,ncol=10;
	int size = nrow*ncol*sizeof(int);
	int *mat1,*mat2,*sum_mat;

	mat1 = (int *)malloc(size);
	mat2 = (int *)malloc(size);
	sum_mat = (int *)malloc(size);

	srand(NULL);

	for(int i=0;i<nrow;i++)
		for(int j=0;j<ncol;j++)
		{
			mat1[i*ncol+j] = rand()%4+1;
			mat2[i*ncol+j] = rand()%5+1;
		}

	int *dev_mat1,*dev_mat2,*dev_sum_mat;

	cudaMalloc((void **)&dev_mat1,size);
	cudaMalloc((void **)&dev_mat2,size);
	cudaMalloc((void **)&dev_sum_mat,size);


	cudaMemcpy(dev_mat1,mat1,size,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_mat2,mat2,size,cudaMemcpyHostToDevice);


	dim3 dimBlock(TILE_SIZE,TILE_SIZE);
	dim3 dimGrid(ceil(ncol/TILE_SIZE),ceil(nrow/TILE_SIZE));

	add_matrix<<<dimGrid,dimBlock>>>(dev_mat1,dev_mat2,dev_sum_mat,nrow,ncol);

	cudaMemcpy(sum_mat,dev_sum_mat,size,cudaMemcpyDeviceToHost);



	printf("Matrix 1\n");
	printMatrix(mat1,nrow,ncol);
	printf("Matrix 2\n");
	printMatrix(mat2,nrow,ncol);
	printf("Matrix Sum\n");
	printMatrix(sum_mat,nrow,ncol);

	cudaFree(dev_mat1);
	cudaFree(dev_mat2);
	cudaFree(dev_sum_mat);

	free(mat1);
	free(mat2);
	free(sum_mat);
}