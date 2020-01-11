#pragma once

template<typename T>
class Array final
{
private:
	T* arr;
	int capacity;
	int size;
	void resize(int newCapacity);
public:
	Array();
	Array(int capacity);
	~Array();
	void insert(const T& value);
	void insert(int index, const T& value);
	void remove(int index);
	const T& operator[](int index) const;
	T& operator[](int index);
	int getSize() const;

	class Iterator
	{
	public:
		Iterator(Array<T>& arr);
		const T& get() const;
		void set(const T& value);
		void insert(const T& value);
		void remove();
		void next();
		void prev();
		void toIndex(int index);
		bool hasNext() const;
		bool hasPrev() const;
	private:
		int indexIter;
		Array<T>& arrIter;
	};

	Iterator iterator();
	const Iterator iterator() const;
};

#include "array.hpp"