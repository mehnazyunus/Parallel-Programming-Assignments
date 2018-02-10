#include <cuda.h>
#include <stdio.h>

int main()
{
	cudaDeviceProp prop;

	int count;
	cudaGetDeviceCount(&count);

	for(int i=0;i<count;++i)
	{
		cudaGetDeviceProperties(&prop,i);
		printf( "--- General Information for device %d ---\n", i );
		printf( "Name:%s\n", prop.name );
		printf( "Compute capability:%d.%d\n", prop.major, prop.minor );
		printf( "Clock rate:%d\n", prop.clockRate );
		printf( "Device copy overlap:" );
		
		if (prop.deviceOverlap)
			printf( "Enabled\n" );
		else
			printf( "Disabled\n" );
		
		printf( "Kernel execution timeout :" );
		
		if (prop.kernelExecTimeoutEnabled)
			printf( "Enabled\n" );
		else
			printf( "Disabled\n" );
		
		printf( "--- Memory Information for device %d ---\n", i );
		printf( "Total global mem:%ld\n", prop.totalGlobalMem );
		printf( "Total constant Mem:%ld\n", prop.totalConstMem );
		
		printf( "Max mem pitch:%ld\n", prop.memPitch );
		printf( "Texture Alignment:%ld\n", prop.textureAlignment );
	}
}
