#include <iostream>
#include "qsort.h"
using namespace std;

int main()
{
	const int size = 100;
	
	int a[size];

	// fill the array with random numbers
	for (int i = 0; i < size; i++)
		a[i] = std::rand() % 200;
	// show the array
	for (int i = 0; i < size; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;

	quicksort(a, a + size, [](const auto& a, const auto& b) { return a < b; });


	for (int i = 0; i < size; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}