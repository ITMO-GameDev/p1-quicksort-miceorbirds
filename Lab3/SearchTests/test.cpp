#include "pch.h"
#include "../search.h"

#include "pch.h"

TEST(DictionaryTest, PutTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 5);

	EXPECT_EQ(dictionary.getSize(), 1);
}

TEST(DictionaryTest, ContainsTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 5);

	EXPECT_TRUE(dictionary.contains(1));
	EXPECT_TRUE(!dictionary.contains(5));
}

TEST(DictionaryTest, RemoveTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 1);
	dictionary.put(2, 2);
	dictionary.remove(1);

	EXPECT_EQ(dictionary.getSize(), 1);
	EXPECT_TRUE(dictionary.contains(2));
	EXPECT_TRUE(!dictionary.contains(1));
}

TEST(DictionaryTest, RemoveUnexistingElementTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 1);

	EXPECT_NO_THROW(dictionary.remove(2));
}

TEST(DictionaryTest, AccessOperatorTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 2);

	EXPECT_EQ(dictionary[1], 2);
}

TEST(DictionaryTest, AccessOperatorUnexistingElementTest)
{
	Dictionary<int, int> dictionary;

	EXPECT_EQ(dictionary[10], 0);
}

TEST(IteratorTest, IteratorKeyAndGetTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 2);

	auto iterator = dictionary.iterator();

	EXPECT_EQ(iterator.key(), 1);
	EXPECT_EQ(iterator.get(), 2);
}

TEST(IteratorTest, IteratorSetTest)
{
	Dictionary<int, int> dictionary;
	dictionary.put(1, 2);

	auto iterator = dictionary.iterator();
	iterator.set(5);

	EXPECT_EQ(iterator.get(), 5);
}

TEST(IteratorTest, ForwardIteratorMovementTest)
{
	Dictionary<int, int> dictionary;
	bool* checkArray = new bool[15]{};

	for (int i = 0; i < 15; i++)
	{
		dictionary.put(i, i);
		checkArray[i] = false;
	}

	auto iterator = dictionary.iterator();

	checkArray[iterator.key()] = true;

	while (iterator.hasNext())
	{
		iterator.next();

		EXPECT_FALSE(checkArray[iterator.key()]);

		checkArray[iterator.key()] = true;
	}

	for (int i = 0; i < 15; i++)
	{
		EXPECT_TRUE(checkArray[i]);
	}
}

TEST(IteratorTest, BackwardIteratorMovementTest)
{
	Dictionary<int, int> dictionary;
	bool* checkArray = new bool[15]{};

	for (int i = 0; i < 15; i++)
	{
		dictionary.put(i, i);
		checkArray[i] = false;
	}

	auto iterator = dictionary.iterator();

	while (iterator.hasNext())
	{
		iterator.next();
	}

	checkArray[iterator.key()] = true;

	while (iterator.hasPrev())
	{
		iterator.prev();


		EXPECT_FALSE(checkArray[iterator.key()]);

		checkArray[iterator.key()] = true;
	}

	for (int i = 0; i < 15; i++)
	{
		EXPECT_TRUE(checkArray[i]);
	}
}