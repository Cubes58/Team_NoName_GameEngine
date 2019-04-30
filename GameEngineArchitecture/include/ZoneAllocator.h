/**
@file ZoneAllocator.h
@brief A class which manages dynamic allocates, for custom containers.
*/
#pragma once

#include "MemoryBlock.h"

/**
	\brief A structure to represent a zone node.
*/
struct ZoneNode {
	MemoryHeaderInformation m_ManagedMemoryBlock;		//!< Manages the memory header information, within the zone node.

	ZoneNode *m_PreviousNode = nullptr;	//!< A pointer that points to the next node in the zone allocator.
	ZoneNode *m_NextNode = nullptr;		//!< A pointer that points to the previous node in the zone allocator.

	/*!
		\brief Constructor.
	*/
	ZoneNode() = default;
	/*!
		\brief Constructor.
		\param p_ManagedMemoryBlock The managed memory block's header information.
		\param p_PreviousNode The previous zone node.
		\param p_NextNode The next zone node.
	*/
	ZoneNode(MemoryHeaderInformation p_ManagedMemoryBlock, ZoneNode *p_PreviousNode = nullptr, ZoneNode *p_NextNode = nullptr)
		: m_ManagedMemoryBlock(p_ManagedMemoryBlock), m_PreviousNode(p_PreviousNode), m_NextNode(p_NextNode) { }

	/*!
		\brief Sets the information of a memory block.
		\param p_ManagedMemoryBlock The managed memory block's header information.
		\param p_PreviousNode The previous zone node.
		\param p_NextNode The next zone node.
		\return Nothing.
	*/
	void SetInformation(MemoryHeaderInformation p_ManagedMemoryBlock, ZoneNode *p_PreviousNode = nullptr, ZoneNode *p_NextNode = nullptr) {
		m_ManagedMemoryBlock = p_ManagedMemoryBlock;
		if (p_PreviousNode != nullptr)
			m_PreviousNode = p_PreviousNode;
		if(p_NextNode != nullptr)
			m_NextNode = p_NextNode;
	}

	/*!
		\brief Copies the ZoneNode data from one instance to another.
		\param p_OtherZoneNode The ZoneNode instance to copy data from, into this instance.
		\return Returns the ZoneNode instance.
	*/
	ZoneNode &operator= (const ZoneNode &p_OtherZoneNode) {
		m_ManagedMemoryBlock = p_OtherZoneNode.m_ManagedMemoryBlock;
		m_PreviousNode = p_OtherZoneNode.m_PreviousNode;
		m_NextNode = p_OtherZoneNode.m_NextNode;

		return *this;
	}
};

/*! \class ZoneAllocator
	\brief A class that manages the dynamic memory allocation of containers.
*/
class ZoneAllocator {
private:
	MemoryBlock m_MemoryBlock;	//<! Manages the memory block the every zone node fits in.
	ZoneNode *m_StartNode = nullptr;	//!< A pointer to a node, in the zone allocator.

public:
	explicit ZoneAllocator(MemoryBlock p_MemoryBlock); //!< Default constructor.
	~ZoneAllocator() = default; //!< Default destructor.

	/*!
		\brief Allocates a zone node (piece of memory) to be used.
		\param p_NumberOfBytes The size of the memory block, in bytes.
		\return A pointer to the zone node, or a nullptr if there isn't enough memory (left), in the zone allocator.
	*/
	ZoneNode *Allocate(unsigned int p_NumberOfBytes);
	/*!
		\brief Frees a zone nodes.
		\param p_ZoneNode A pointer to the zone node to free.
	*/
	void FreeMemory(ZoneNode *p_ZoneNode);
};