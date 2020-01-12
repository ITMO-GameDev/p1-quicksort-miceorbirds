#include <iostream>
#include "memorym.h"

int main()
{
	MemoryAllocator allocator;
	allocator.init();

	int* pi = allocator.alloc(sizeof(int));
	double* pd = allocator.alloc(sizeof(double));
	int* pa = allocator.alloc(10 * sizeof(int));

	allocator.dumpStat();
	allocator.dumpBlocks();

	allocator.free(pa);
	allocator.free(pd);
	allocator.free(pi);

	allocator.destroy();

}