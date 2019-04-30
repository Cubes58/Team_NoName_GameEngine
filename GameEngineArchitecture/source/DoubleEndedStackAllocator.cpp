#include "DoubleEndedStackAllocator.h"

#include <cstdlib>
#include <string>

DoubleEndedStackAllocator::DoubleEndedStackAllocator(unsigned int p_StackSize) {
	m_MemoryHead.m_StartPointer = (Marker)malloc(p_StackSize);
	m_MemoryHead.m_EndPointer = m_MemoryHead.m_StartPointer;

	m_MemoryTail.m_StartPointer = (Marker)(m_MemoryHead.m_StartPointer + p_StackSize);
	m_MemoryTail.m_EndPointer = m_MemoryTail.m_StartPointer;
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator() {
	if(m_MemoryHead.m_StartPointer != nullptr) {
		delete m_MemoryHead.m_StartPointer;
		m_MemoryHead.m_StartPointer = nullptr;
	}
}

MemoryBlock DoubleEndedStackAllocator::Allocate(unsigned int p_NumberOfBytes, bool p_Static) {
	auto calculateMemoryBlock = [](unsigned char *p_HeadOfMemoryblock, int p_NumberOfBytes)->MemoryBlock {
		MemoryBlock memoryBlock;
		memoryBlock.m_StartPointer = p_HeadOfMemoryblock;
		memoryBlock.m_EndPointer = p_HeadOfMemoryblock + p_NumberOfBytes;

		if(p_NumberOfBytes < 0)
			p_NumberOfBytes *= -1;
		memoryBlock.m_Size = p_NumberOfBytes;

		return memoryBlock;
	};

	if(p_Static) {
		m_MemoryHead.m_EndPointer += p_NumberOfBytes;
		m_MemoryHead.m_Size += p_NumberOfBytes;
		return calculateMemoryBlock(m_MemoryHead.m_EndPointer, p_NumberOfBytes);
	}

	m_MemoryTail.m_EndPointer -= p_NumberOfBytes;
	m_MemoryTail.m_Size += p_NumberOfBytes;
	return calculateMemoryBlock(m_MemoryTail.m_EndPointer, -(int)p_NumberOfBytes);
}

void DoubleEndedStackAllocator::FreeMemory(Marker p_Marker) {
	auto freeMemory = [&p_Marker](MemoryBlock &p_Memoryblock, int p_NumberOfBytes, bool p_Static = false)->void {
		// Clear the memory value[s] to 0.
		memset(p_Memoryblock.m_EndPointer, 0, p_NumberOfBytes);

		if(p_Static)
			p_NumberOfBytes *= -1;
		p_Memoryblock.m_EndPointer += p_NumberOfBytes;

		if(!p_Static)
			p_NumberOfBytes *= -1;
		p_Memoryblock.m_Size += p_NumberOfBytes;
	};

	// Static (remove memory from the head).
	if(p_Marker <= m_MemoryHead.m_EndPointer)
		return freeMemory(m_MemoryHead, -(m_MemoryHead.m_StartPointer - p_Marker), true);

	// Dynamic (remove memory from the tail).
	freeMemory(m_MemoryTail, -(p_Marker - m_MemoryTail.m_StartPointer), false);
}