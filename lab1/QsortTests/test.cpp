#include "pch.h"
#include "../Lab1/qsort.h"

const auto compIntLess = [](int a, int b) { return a < b; };

template<typename T>
bool isEqual(T* first, T* second, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (first[i] != second[i])
        {
            return false;
        }
    }
    return true;
}

TEST(SortTest, FindPivotValueTest)
{
    const int length = 7;
    int arrayTest[length] = { 6, 5, 4, 3, 2, 1, 0 };

    int pivotValue = mid(arrayTest, arrayTest[0], arrayTest[length-1], compIntLess);
    ASSERT_EQ(pivotValue, 3);
}


TEST(SortTest, SingleElementTest)
{
    const int length = 1;
    int arrayTest[length] = { 12 };
    int expectedArray[length] = { 12 };

    quicksort(arrayTest, arrayTest + length, compIntLess);
    ASSERT_EQ(isEqual(expectedArray, arrayTest, length), true);
}
TEST(SortTest, SpecArrayIsSortedTest)
{
    const int length = 7;
    int arrayTest[length] = { 6, 5, 4, 3, 2, 1, 0 };
    int expectedArray[length] = { 0, 1, 2, 3, 4, 5, 6 };

    quicksort(arrayTest, arrayTest + length, compIntLess);
    ASSERT_EQ(isEqual(expectedArray, arrayTest, length), true);
}

TEST(SortTest, PresortedArrayTest)
{
    const int length = 7;
    int arrayTest[length] = { 0, 1, 2, 3, 4, 5, 6 };
    int expectedArray[length] = { 0, 1, 2, 3, 4, 5, 6 };

    quicksort(arrayTest, arrayTest + length, compIntLess);
    ASSERT_EQ(isEqual(expectedArray, arrayTest, length), true);
}

