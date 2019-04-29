/**
@file DoubleEndedStackAllocator.h
@brief Manages a large chunk of memory, which can grow inward from either direciton.
*/
#pragma once

#include "MemoryBlock.h"


/*! \class DoubleEndedStackAllocator
	\brief Manages a large chunk of memory, which can grow inward from either direciton.
*/

class DoubleEndedStackAllocator {
private:
	MemoryBlock m_MemoryHead;	//!<
	MemoryBlock m_MemoryTail;	//!<

public:
	/*!
		\brief Constructor.
		\param p_StackSize The size of the stack, in Bytes.
	*/
	explicit DoubleEndedStackAllocator(unsigned int p_StackSize);
	/*!
		\brief Destructor.
	*/
	~DoubleEndedStackAllocator();

	/*!
		\brief Allocates a requested amount of its stack memory, when requested.
		\param p_NumberOfBytes The number of bytes being requested.
		\param p_Static Whether the memory being requested will be used to store static or dynamic memory.
		\return Returns a MemoryBlock object, which has information about the allocated memory.
	*/
	MemoryBlock Allocate(unsigned int p_NumberOfBytes, bool p_Static = false);
	/*!
		\brief Frees allocated memory, to a marker, when requested to.
		\param p_Marker the marker to free the memory to.
		\return Returns Nothing.
	*/
	void FreeMemory(Marker p_Marker);
};