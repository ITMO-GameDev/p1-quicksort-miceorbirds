#pragma once

template<typename T, typename Compare>
void quicksort(T* first, T* last, Compare comp);

template<typename T, typename Compare>
void quickinsort(T* arr, int lo, int hi, Compare comp);

template<typename T, typename Compare>
void insertionsort(T* arr, int lo, int hi, Compare comp);

template <typename T, typename Compare>
T mid(T* arr, int first, int last, Compare comp);

template <typename T, typename Compare>
int partition(T* arr, T lo, T hi, T p, Compare comp);

template <typename T>
void swapElem(T& x, T& y);

#include "qsort.hpp"