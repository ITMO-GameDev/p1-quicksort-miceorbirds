#include <iostream>
#include "array.h"

int main()
{
	Array<int> a;

	for(int i = 0; i < 10; i++)
		a.insert(i);
	for (int i = 0; i < a.getSize(); i++)
		a[i] *= 2;
	for (auto it = a.iterator(); it.hasNext(); it.next())
		std::cout << it.get() << std::endl;
}