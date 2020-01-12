#pragma once
#include <cmath>
#include <cassert>
#define NOMINMAX
#include <iostream>
#include <limits>  /* limits_max */
#include <windows.h>

int MemoryAllocator::align(int size)
{
	auto value = pow(2, ceil(log2(size)));
	return value >= 16 ? value : 16;
}

MemoryAllocator::MemoryAllocator() : isInit(false)
{
}

MemoryAllocator::~MemoryAllocator()
{
	this->MemoryAllocator::destroy();
}

void MemoryAllocator::init()
{
	int start = 16;

	for (; start <= 512; start *= 2)
	{
		auto allocator = new FixedAlloc(start);
		allocator->init();

		FSA_alloc.insert_or_assign(start, allocator);
	}
	auto freelist = new FreeAlloc(52428800, FreeAlloc::PlacementPolicy::FIND_FIRST);
	freelist->init();
	FL_alloc.insert_or_assign(0, freelist);

	isInit = true;
}

void* MemoryAllocator::alloc(size_t size)
{
#ifndef NDEBUG
	assert(isInit);
#endif
	if (!isInit) return nullptr;
	if (size > 512)
	{
		for (auto it = FL_alloc.begin(); it != FL_alloc.end(); ++it)
		{
			if (it->second->getFreeSize() > size)
			{
				auto mem = it->second->allocate(size);
				std::cout << mem << std::endl;
				dumpFLMap.insert_or_assign(mem, it->second);
				return mem;

			}
		}
		auto freelist = new FreeAlloc(52428800, FreeAlloc::PlacementPolicy::FIND_FIRST);
		freelist->init();
		FL_alloc.insert_or_assign(FL_alloc.size() + 1, freelist);
		auto mem = freelist->allocate(size);
		dumpFLMap.insert_or_assign(mem, freelist);
		return mem;

	}
	auto alignedSize = align(size);
	auto allocator = FSA_alloc[alignedSize];
	return  allocator->alloc();
}

void MemoryAllocator::free(void* ptr)
{
#ifndef NDEBUG
	assert(isInit);
#endif
	if (!isInit) return;

	for (auto it = FSA_alloc.begin(); it != FSA_alloc.end(); ++it)
	{
		if (it->second->free(ptr))
			return;
	}

	if (dumpFLMap.count(ptr))
	{

		dumpFLMap[ptr]->free(ptr);
		dumpFLMap.erase(ptr);
		return;
	}
}

void MemoryAllocator::destroy()
{
#ifndef NDEBUG
	assert(isInit);
#endif
	if (!isInit) return;
	for (auto it = FSA_alloc.begin(); it != FSA_alloc.end(); ++it)
	{
		it->second->destroy();
	}
	for (auto it = FL_alloc.begin(); it != FL_alloc.end(); ++it)
	{
		it->second->destroy();
	}
	FSA_alloc.clear();
	FL_alloc.clear();
	dumpFLMap.clear();
	isInit = false;
}

void MemoryAllocator::dumpBlocks() const
{
#ifndef NDEBUG
	assert(isInit);
#endif
	if (!isInit) return;
	for (auto it = FSA_alloc.begin(); it != FSA_alloc.end(); ++it)
	{
		it->second->dumpBlocks();
	}
}

FixedAlloc::FixedAlloc(const std::size_t chunkSize) : m_chunkSize(chunkSize), m_freeList()
{
	isInit = false;
}

FixedAlloc::~FixedAlloc()
{
	this->FixedAlloc::destroy();
}

void FixedAlloc::init()
{
	request_page();
	isInit = true;
}

void* FixedAlloc::alloc()
{

	if (m_freeList.head == nullptr)
	{
		request_page();
	}
	Node* freePosition = m_freeList.pop();
	dumpSet.insert(freePosition);
	return static_cast<void*>(freePosition);
}

bool FixedAlloc::free(void* ptr)
{
	auto contains = dumpSet.erase(ptr);
	if (contains) {
		m_freeList.push(static_cast<Node*>(ptr));
	}
	return contains;
}

void FixedAlloc::destroy()
{
	isInit = false;
	dumpSet.clear();

}
void FixedAlloc::dumpBlocks()
{
	for (auto val : dumpSet) {
		std::cout << val << " " << m_chunkSize << std::endl;
	}

}

void FixedAlloc::chunk_page(void* page)
{
	const int nChunks = pageSize / m_chunkSize;
	for (int i = 0; i < nChunks; ++i) {
		const std::size_t address = reinterpret_cast<std::size_t>(page) + i * m_chunkSize;
		m_freeList.push(reinterpret_cast<Node*>(address));
	}
}

void* FixedAlloc::request_page()
{
	const auto page = VirtualAlloc(nullptr, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	pages.push_back(page);
	chunk_page(page);
	return page;
}

FreeAlloc::FreeAlloc(const std::size_t totalSize, const PlacementPolicy pPolicy)
	: m_totalSize(totalSize), m_used(0), m_pPolicy(pPolicy)
{
}

void FreeAlloc::init()
{
	if (m_start_ptr != nullptr)
	{
		free(m_start_ptr);
		m_start_ptr = nullptr;
	}
	m_start_ptr = VirtualAlloc(nullptr, this->m_totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	this->reset();
}

void FreeAlloc::destroy()
{
	VirtualFree(m_start_ptr, 0, MEM_RELEASE);
	m_used = 0;
	m_start_ptr = nullptr;
}

std::size_t FreeAlloc::getFreeSize() const
{
	return m_totalSize - m_used;
}

FreeAlloc::~FreeAlloc()
{
	this->destroy();
}

void* FreeAlloc::allocate(const std::size_t size, const std::size_t alignment)
{
	const std::size_t allocationHeaderSize = sizeof(AllocationHeader);
	const std::size_t freeHeaderSize = sizeof(FreeHeader);

	// Search through the free list for a free block that has enough space to allocate our data
	std::size_t padding;
	Node* affectedNode,
		* previousNode;
	this->find(size, alignment, padding, previousNode, affectedNode);


	const std::size_t alignmentPadding = padding - allocationHeaderSize;
	const std::size_t requiredSize = size + padding;
	const std::size_t rest = affectedNode->data.blockSize - requiredSize;

	if (rest > 0)
	{
		// We have to split the block into the data block and a free block of size 'rest'
		Node* newFreeNode = reinterpret_cast<Node*>(std::size_t(affectedNode) + requiredSize);
		newFreeNode->data.blockSize = rest;
		m_freeList.insert(affectedNode, newFreeNode);
	}
	m_freeList.remove(previousNode, affectedNode);

	// Setup data block
	const std::size_t headerAddress = std::size_t(affectedNode) + alignmentPadding;
	const std::size_t dataAddress = headerAddress + allocationHeaderSize;
	reinterpret_cast<AllocationHeader*>(headerAddress)->blockSize = requiredSize;
	reinterpret_cast<AllocationHeader*>(headerAddress)->padding = alignmentPadding;

	m_used += requiredSize;


	return reinterpret_cast<void*>(dataAddress);
}

void FreeAlloc::find(const std::size_t size, const std::size_t alignment, std::size_t& padding,
	Node*& previousNode, Node*& foundNode) const
{
	switch (m_pPolicy)
	{
	case FIND_FIRST:
		find_first(size, alignment, padding, previousNode, foundNode);
		break;
	case FIND_BEST:
		find_best(size, alignment, padding, previousNode, foundNode);
		break;
	}
}

void FreeAlloc::find_first(const std::size_t size, const std::size_t alignment, std::size_t& padding,
	Node*& previousNode, Node*& foundNode) const
{
	Node* it = m_freeList.head,
		* itPrev = nullptr;

	while (it != nullptr)
	{
		const std::size_t requiredSpace = size;
		if (it->data.blockSize >= requiredSpace)
		{
			break;
		}
		itPrev = it;
		it = it->next;
	}
	previousNode = itPrev;
	foundNode = it;
}

void FreeAlloc::find_best(const std::size_t size, const std::size_t alignment, std::size_t& padding,
	Node*& previousNode, Node*& foundNode) const
{
#undef max
	auto smallestDiff = std::numeric_limits<std::size_t>::max();
	Node* bestBlock = nullptr;
	Node* it = m_freeList.head,
		* itPrev = nullptr;
	while (it != nullptr)
	{
		const std::size_t requiredSpace = size;
		if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
		{
			bestBlock = it;
		}
		itPrev = it;
		it = it->next;
	}
	previousNode = itPrev;
	foundNode = bestBlock;
}

void FreeAlloc::free(void* ptr)
{
	// Insert it in a sorted position by the address number
	const auto currentAddress = std::size_t(ptr);
	const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);
	const AllocationHeader* allocationHeader{ reinterpret_cast<AllocationHeader*>(headerAddress) };

	Node* freeNode = reinterpret_cast<Node*>(headerAddress);
	freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
	freeNode->next = nullptr;

	Node* it = m_freeList.head;
	Node* itPrev = nullptr;
	while (it != nullptr)
	{
		if (ptr < it)
		{
			m_freeList.insert(itPrev, freeNode);
			break;
		}
		itPrev = it;
		it = it->next;
	}

	m_used -= freeNode->data.blockSize;

	// Merge contiguous nodes
	coalescence(itPrev, freeNode);

}

void FreeAlloc::coalescence(Node* previousNode, Node* freeNode)
{
	if (freeNode->next != nullptr &&
		std::size_t(freeNode) + freeNode->data.blockSize == std::size_t(freeNode->next))
	{
		freeNode->data.blockSize += freeNode->next->data.blockSize;
		m_freeList.remove(freeNode, freeNode->next);
	}

	if (previousNode != nullptr &&
		std::size_t(previousNode) + previousNode->data.blockSize == std::size_t(freeNode))
	{
		previousNode->data.blockSize += freeNode->data.blockSize;
		m_freeList.remove(previousNode, freeNode);

	}
}

void FreeAlloc::reset()
{
	m_used = 0;
	Node* firstNode = static_cast<Node*>(m_start_ptr);
	firstNode->data.blockSize = m_totalSize;
	firstNode->next = nullptr;
	m_freeList.head = nullptr;
	m_freeList.insert(nullptr, firstNode);
}

template <class T>
void LinkedList<T>::insert(Node* previousNode, Node* newNode)
{
	if (previousNode == nullptr)
	{
		if (head != nullptr)
		{
			newNode->next = head;
		}
		else
		{
			newNode->next = nullptr;
		}
		head = newNode;
	}
	else
	{
		if (previousNode->next == nullptr)
		{
			previousNode->next = newNode;
			newNode->next = nullptr;
		}
		else
		{
			newNode->next = previousNode->next;
			previousNode->next = newNode;
		}
	}
}

template <class T>
void LinkedList<T>::remove(Node* previousNode, Node* deleteNode)
{
	if (previousNode == nullptr)
	{
		if (deleteNode->next == nullptr)
		{
			head = nullptr;
		}
		else
		{
			head = deleteNode->next;
		}
	}
	else
	{
		previousNode->next = deleteNode->next;
	}
}

template <class T>
void StackLinkedList<T>::push(Node* newNode) {
	newNode->next = head;
	head = newNode;
}

template <class T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
	Node* top = head;
	head = head->next;
	return top;
}