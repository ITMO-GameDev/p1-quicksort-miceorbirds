#pragma once

const int BASECAPACITY = 8;
const int MULTIPLYCAPACITY = 2;

template<typename T>
Array<T>::Array() : capacity(BASECAPACITY), size(0)
{
	arr = new T[capacity];
}

template<typename T>
Array<T>::Array(int capacity) : capacity(capacity), size(0)
{
	arr = new T[capacity];
}

template <typename T>
Array<T>::~Array()
{
}

template <typename T>
void Array<T>::insert(const T& value)
{
	if (size == capacity)
	{
		resize(size * MULTIPLYCAPACITY);
	}
	size++;
	arr[size - 1] = value;
}

template <typename T>
void Array<T>::insert(int index, const T& value)
{
	if (size == capacity)
	{
		resize(capacity * MULTIPLYCAPACITY);
	}
	for (int i = size; i > index; i--)
	{
		arr[i] = std::move(arr[i - 1]);
	}
	arr[index] = value;
	size++;
}

template <typename T>
void Array<T>::remove(int index)
{
	for (int i = index; i < size; i++)
	{
		arr[i] = std::move(arr[i + 1]);
	}
	size--;
}

template <typename T>
const T& Array<T>::operator[](int index) const
{
	return arr[index];
}

template <typename T>
T& Array<T>::operator[](int index)
{
	return arr[index];
}

template <typename T>
int Array<T>::getSize() const
{
	return size;
}

// ITERATOR
template <typename T>
Array<T>::Iterator::Iterator(Array<T>& arr) : indexIter(0), arrIter(arr)
{
}

template<typename T>
const T& Array<T>::Iterator::get() const
{
	return arrIter[indexIter];
}

template<typename T>
void Array<T>::Iterator::set(const T& value)
{
	arrIter[indexIter] = value;
}

template<typename T>
void Array<T>::Iterator::insert(const T& value)
{
	arrIter.insert(indexIter, value);
}

template<typename T>
void Array<T>::Iterator::remove()
{
	arrIter.remove(indexIter);
}

template<typename T>
void Array<T>::Iterator::next()
{
	indexIter++;
}

template<typename T>
void Array<T>::Iterator::prev()
{
	indexIter--;
}

template<typename T>
void Array<T>::Iterator::toIndex(int index)
{
	this->indexIter = index;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const
{
	return indexIter <  this->arrIter.getSize() - 1;
}

template<typename T>
bool Array<T>::Iterator::hasPrev() const
{
	return indexIter > 0;
}

template<typename T>
typename Array<T>::Iterator Array<T>::iterator()
{
	Iterator iterator(*this);
	return iterator;
}

template<typename T>
const typename Array<T>::Iterator Array<T>::iterator() const
{
	Iterator iterator(*this);
	return iterator;
}

template <typename T>
void Array<T>::resize(int newCapacity)
{
	Array tempArr(newCapacity);

	for (int i = 0; i < size; i++)
	{
		tempArr.arr[i] = std::move(arr[i]);
	}

	tempArr.size = size;
	std::swap(*this, tempArr);
}