#pragma once
constexpr int QUICKSORT_CUTOFF = 25;

template<typename T, typename Compare>
void quicksort(T* first, T* last, Compare comp)
{
	int f = 0;
	//we need the last index of the array
	int l = last-first-1;
	quickinsort(first, f, l, comp);
}

template<typename T, typename Compare>
void quickinsort(T* arr, int lo, int hi, Compare comp)
{
	while (lo < hi)
	{
		if (hi - lo + 1 <= QUICKSORT_CUTOFF)
		{
			// Use insertion sort for small parts
			insertionsort(arr, lo, hi, comp);
			return;
		}
		T pValue = mid(arr, lo, hi, comp);
		int pIndex = partition(arr, lo, hi, pValue, comp);
		// sort less part recursively and bigger part iteratively
		if (pIndex - lo < hi - pIndex)
		{
			quickinsort(arr, lo, pIndex - 1, comp);
			lo = pIndex;
		}
		else
		{
			quickinsort(arr, pIndex, hi, comp);
			hi = pIndex - 1;
		}
	}
}

template <typename T, typename Compare>
int partition(T* arr, T lo, T hi, T p, Compare comp)
{
	int i = lo;
	int j = hi;
	while (i<j) {

		while (comp(arr[i], p)) i++;
		while (comp(p, arr[j]) || arr[j] == p) j--;

		if (i >= j) break;

		swapElem(arr[i], arr[j]);
	}
	return i;
}

template<typename T, typename Compare>
void insertionsort(T* arr, int lo, int hi, Compare comp)
{
	T temp;
	int i = 0;
	for (int k = lo + 1; k <= hi; k++)
	{
		temp = arr[k];
		i = k;
		while (i > lo && (comp(temp, arr[i - 1]) || arr[i - 1] == temp) ) 
		{
			arr[i] = arr[i - 1];
			i--;
		}
		arr[i] = temp;
	}
}

//find median of first, middle and last items of the array
template <typename T, typename Compare>
T mid(T* arr, int lo, int hi, Compare comp)
{
	int center = (lo + hi) / 2;
	if (comp(arr[center], arr[lo])) swapElem(arr[lo], arr[center]);
	if (comp(arr[hi], arr[lo])) swapElem(arr[lo], arr[hi]);
	if (comp(arr[hi], arr[center])) swapElem(arr[center], arr[hi]);
	// for pivot we need a value of that item
	return arr[center];
}

template <typename T>
void swapElem(T& x, T& y)
{
	if (x == y) return;
	else
	{
		T temp;
		temp = x;
		x = y;
		y = temp;
	}
}
