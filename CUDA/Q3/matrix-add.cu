
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 10
#define TILE_SIZE 16.0


__global__ void add_matrix(int* dev_mat1, int* dev_mat2, int* dev_sum_mat, int nrow, int ncol)
{
	int row=blockIdx.y*blockDim.y+threadIdx.y;
	int col=blockIdx.x*blockDim.x+threadIdx.x;
	
	if(row<nrow && col<ncol)
	dev_sum_mat[row*ncol+col]=dev_mat1[row*ncol+col]+dev_mat2[row*ncol+col];
	
}

void printMatrix(int* mat,int ncol)
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		printf("%d ",mat[i*ncol+j]);
	    
		printf("\n");
	}
}

int main()
{	
	int nrow = 100,ncol = 100;
	
	int *mat1,*mat2,*sum_mat;

	int size=nrow*ncol*sizeof(int);
	mat1 = (int *)malloc(size);
	mat2 = (int *)malloc(size);
	sum_mat = (int *)malloc(size);
	
	srand(time(NULL));
	
	for(int i=0;i<nrow;i++)
		for(int j=0;j<ncol;j++)
		{
			mat1[i*ncol+j] = rand()%4+1;
			mat2[i*ncol+j] = rand()%5+1;
		}

	
	int* dev_mat1, *dev_mat2, *dev_sum_mat;
	
	cudaMalloc((void**)&dev_mat1,size);
	cudaMalloc((void**)&dev_mat2,size);
	cudaMalloc((void**)&dev_sum_mat,size);
	
	cudaMemcpy(dev_mat1,mat1,size,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_mat2,mat2,size,cudaMemcpyHostToDevice);
	
	dim3 dimBlock(TILE_SIZE,TILE_SIZE);
	dim3 dimGrid(ceil(ncol/TILE_SIZE),ceil(nrow/TILE_SIZE));
	add_matrix<<<dimGrid,dimBlock>>>(dev_mat1,dev_mat2,dev_sum_mat,nrow,ncol);
	
	cudaMemcpy(sum_mat,dev_sum_mat,size,cudaMemcpyDeviceToHost);
	printf("Matrix 1\n");
	printMatrix(mat1,ncol);
	printf("Matrix 2\n");
	printMatrix(mat2,ncol);
	printf("Matrix Sum for first N x N matrix\n");
	printMatrix(sum_mat,ncol);
	
	cudaFree(dev_mat1);
	cudaFree(dev_mat2);
	cudaFree(dev_sum_mat);

	free(mat1);
	free(mat2);
	free(sum_mat);	
}
