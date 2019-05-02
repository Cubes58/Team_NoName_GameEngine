/**
@file DoubleEndedStackAllocator.h
@brief Manages a large chunk of memory, which can grow inward from either direction.
*/
#pragma once

#include "MemoryBlock.h"


/*! \class DoubleEndedStackAllocator
	\brief Manages a large chunk of memory, which can grow inward from either direction.
*/

class DoubleEndedStackAllocator {
private:
	MemoryBlock m_MemoryHead;	//!< A memory block, representing the head's chunk of allocated memory.
	MemoryBlock m_MemoryTail;	//!< A memory block, representing the tail's chunk of allocated memory.

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