/**
@file MemoryManager.h
@brief A singleton memory manager class.
*/
#pragma once

#include "MemoryBlock.h"
#include "DoubleEndedStackAllocator.h"
#include "ZoneAllocator.h"

#define NUMBER_OF_BYTES_PER_MEGABYTE 1000000u
#define STACK_SIZE_IN_MEGABYTES 512u
#define ZONE_SIZE_IN_MEGABYTES 256u

#define MemoryManagerInstance MemoryManager::Instance()

/*! \class MemoryManager
	\brief A memory manager class that allocates and frees memory.
*/

class MemoryManager {
private:
	DoubleEndedStackAllocator m_DoubleEndedStackAllocator;	//!< A large chunk of memory created at the start of the program.
	ZoneAllocator m_ZoneAllocator;	// A chunk of memory, which splits into different sized nodes, to manage dynamic memory better.
	
	MemoryManager();	//!< Constructor.
	~MemoryManager();	//!< Destructor.

public:
	/*!
		\brief Singleton instance.
		\return Returns the singletone instance, of this class.
	*/
	static MemoryManager &Instance();

	/*!
		\brief Allocates a chunk of memory, from the stack.
		\param p_NumberOfBytes The number of bytes to allocate.
		\param p_Static Whether the data that's going to be stored on the static is static, or not.
		\return Returns the memory block, allocated from the stack.
	*/
	MemoryBlock StackAllocate(unsigned int p_NumberOfBytes, bool p_Static = false);
	/*!
		\brief Allocates memory, for a zone node.
		\param p_NumberOfBytes The number of bytes being requested.
		\return Returns a pointer to zone node, to store data in. 
	*/
	ZoneNode *ZoneAllocate(unsigned int p_NumberOfBytes);

	/*!
		\brief Frees memory allocated on the stack, to a marker.
		\param p_Marker The marker to free to.
		\return Nothing.
	*/
	void FreeStackMemory(Marker p_Marker);
	/*!
		\brief Frees a memory zone.
		\param p_ZoneNode The memory zone to free.
		\return Nothing.
	*/
	void FreeZoneMemory(ZoneNode *p_ZoneNode);

	/*!
		\brief Clears all data allocated by this class.
		\return Nothing.
	*/
	void Clear();
};