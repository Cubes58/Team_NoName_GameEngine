/**
@file MemoryBlock.h
@brief Contians the different ways a (custom) memory block.
*/
#pragma once

using Marker = unsigned char *;	//!< Using Marker as an unsigned char *

/**
	\brief A structure to represent a memory block.
*/
struct MemoryBlock {
	Marker m_StartPointer = nullptr;	//!< A pointer to the start of the memory block.
	Marker m_EndPointer = nullptr;		//!< A pointer to the end of the memory block.
	unsigned int m_Size = 0;			//!< The size of the memory block.

	MemoryBlock() = default;
	explicit MemoryBlock(unsigned int p_Size, Marker p_StartPointer) 
		: m_Size(p_Size), m_StartPointer(p_StartPointer), m_EndPointer(p_StartPointer + p_Size) {  }
};

/**
	\brief A structure to represent memory header information.
*/
struct MemoryHeaderInformation {
	Marker m_StartOfHeader = nullptr;	//!< A pointer to the start of the header.
	Marker m_EndOfHeader = nullptr;		//!< A pointer to the end of the header.

	Marker m_StartOfMemoryBlock = nullptr;	//!< A pointer to the start of the memory block, used to store the element data.
	Marker m_EndOfMemoryBlock = nullptr;	//!< A pointer to the end of the memory block, used to store the element data.

	unsigned int m_SizeOfManagedData = 0;	//!< The size of the data being managed.
	bool m_Occupied = false;				//!< Whether this memory block is occupied or not.

	/*!
		\brief Constructor.
	*/
	MemoryHeaderInformation() = default;
	/*!
		\brief Constructor.
		\param p_StartPointer A pointer to the start of the memory block.
		\param p_SizeOfManagedData The size of the memory block.
		\param p_Occupied Whether the memory block is occupied, or not.
	*/
	explicit MemoryHeaderInformation(Marker p_StartPointer, unsigned int p_SizeOfManagedData, bool p_Occupied = false) {
		m_StartOfHeader = p_StartPointer;
		m_EndOfHeader = m_StartOfHeader + sizeof(MemoryHeaderInformation) + (sizeof(MemoryHeaderInformation*) * 2u);	// Plus the two zone node pointers, stored in the header.

		m_SizeOfManagedData = p_SizeOfManagedData;
		m_Occupied = p_Occupied;

		m_StartOfMemoryBlock = m_EndOfHeader;
		m_EndOfMemoryBlock = m_StartOfMemoryBlock + p_SizeOfManagedData;
	}
};