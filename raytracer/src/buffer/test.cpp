#ifndef __BUFFER_TEST
#define __BUFFER_TEST

#include "Buffer.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	size_t width = 10;
	size_t height = 10;
	Buffer<int> intBuffer = Buffer<int>(width, height);

	for(size_t j=0; j<height; j++)
		for(size_t i=0; i<width; i++)
			intBuffer.at(i,j) = 0;

	intBuffer.at(4,4) = 0x1;

	for(size_t j=0; j<height; j++)
	{
		for(size_t i=0; i<width; i++)
			printf("%d ", intBuffer.at(i,j));
		printf("\n");
	}

	return 0;
}

#endif
