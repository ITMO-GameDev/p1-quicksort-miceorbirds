#pragma once
#include <map>
#include <iostream>
#include <vector>
#include <set>


template <class T>
class StackLinkedList {
public:
	struct Node {
		T data;
		Node* next;
	};

	Node* head;
public:
	StackLinkedList() = default;
	StackLinkedList(StackLinkedList& stackLinkedList) = delete;
	void push(Node* newNode);
	Node* pop();
};

template <class T>
class LinkedList
{
public:
	struct Node
	{
		T data;
		Node* next;
	};

	Node* head;

public:
	LinkedList() = default;

	void insert(Node* previousNode, Node* newNode);
	void remove(Node* previousNode, Node* deleteNode);
};

class FreeAlloc
{
public:
	enum PlacementPolicy
	{
		FIND_FIRST,
		FIND_BEST
	};

private:

	struct FreeHeader
	{
		std::size_t blockSize;
	};

	struct AllocationHeader
	{
		std::size_t blockSize;
		char padding;
	};

	typedef LinkedList<FreeHeader>::Node Node;

	std::size_t m_totalSize;
	std::size_t m_used;
	void* m_start_ptr = nullptr;
	PlacementPolicy m_pPolicy;
	LinkedList<FreeHeader> m_freeList;

public:
	FreeAlloc(std::size_t totalSize, PlacementPolicy pPolicy);

	virtual ~FreeAlloc();

	void* allocate(std::size_t size, std::size_t alignment = 8);

	void free(void* ptr);

	void init();

	void destroy();

	virtual void reset();
	std::size_t getFreeSize() const;
private:
	FreeAlloc(FreeAlloc& freeListAllocator);

	void coalescence(Node* previousNode, Node* freeNode);

	void find(std::size_t size, std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode) const;
	void find_best(std::size_t size, std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode) const;
	void find_first(std::size_t size, std::size_t alignment, std::size_t& padding, Node*& previousNode,
		Node*& foundNode) const;
};

class FixedAlloc
{
protected:
	std::size_t m_chunkSize;
	std::vector<void*> pages;
	std::set<void*> dumpSet;
	const static size_t pageSize = 4096;
	bool isInit;

	struct  FreeHeader {
	};
	using Node = StackLinkedList<FreeHeader>::Node;
	StackLinkedList<FreeHeader> m_freeList;

	void* m_start_ptr = nullptr;


	virtual void* request_page();
	virtual  void chunk_page(void* page);

public:
	FixedAlloc(std::size_t chunkSize);

	virtual ~FixedAlloc();
	virtual void init();
	virtual void* alloc();
	virtual bool free(void* ptr);
	virtual void destroy();
	virtual void dumpBlocks();
};



class MemoryAllocator
{
private:
	std::map<int, FixedAlloc*> FSA_alloc;
	std::map<int, FreeAlloc*> FL_alloc;
	std::map<void*, FreeAlloc*> dumpFLMap;
	bool isInit;
	static int align(int size);
public:
	MemoryAllocator();
	virtual ~MemoryAllocator();
	virtual void init();
	virtual void destroy();
	virtual void* alloc(size_t size);
	virtual void free(void* p);
	virtual void dumpStat() const;

	#ifndef NDEBUG
	virtual void dumpBlocks() const;
	#endif


};

#include "memorym.hpp"