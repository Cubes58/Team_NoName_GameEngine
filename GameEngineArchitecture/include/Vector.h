/**
@file Vector.h
@brief A vector container class.
*/
#pragma once

#include <list>

#include "MemoryManager.h"

#define VECTOR_GROWTH_SCALAR 2u	//!< The vector's capacity growth scalar, when the vector needs to find more space, to fit more elements in.

//! Custom Container.
namespace CC {
/*! \class Vector
	\brief A container class that stores elements in a contiguous block of memory.
*/
template <typename Type>
class Vector {
private:
	std::list<unsigned int> m_EmptySpace;	//!< A list of spaces that are free, within the allocated block of memory. 
	ZoneNode *m_MemoryNode = nullptr;		//!< Stores a pointer to its zone node.
	Type *m_Data = nullptr;					//!< Stores a pointer to the data.

	unsigned int m_Size;					//!< The size of the vector, including the null spaces.
	unsigned int m_NumberOfElements;		//!< The total number of elements in the vector.
	unsigned int m_Capacity;				//!< The max size the vector can be, before it must allocate more memory (grow).

public:
	/*!
		\brief Constructor.
		\param p_Capacity Number of elements the reserved memory can fit within it.
	*/
	Vector(unsigned int p_Capacity = 150u) : m_Capacity(p_Capacity), m_Size(0u), m_NumberOfElements(0u) {
		m_MemoryNode = MemoryManagerInstance.ZoneAllocate(sizeof(Type) * m_Capacity);
		m_Data = reinterpret_cast<Type *>(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock);
	}
	/*!
		\brief Destructor.
	*/
	~Vector() {
		MemoryManagerInstance.FreeZoneMemory(m_MemoryNode);
	}

	/*!
		\brief Gets data from the vector.
		\param p_Index the index value, of the element.
		\return Returns the element requested, otherwise nullptr if it couldn't be found.
	*/
	Type *At(unsigned int p_Index) {
		bool validElement = true;
		for(const auto &emptySpace : m_EmptySpace)
			if(emptySpace == p_Index)
				validElement = false;


		if(validElement && p_Index < m_Capacity)
			return m_Data + p_Index;

		return nullptr;
	}

	/*!
		\brief Adds an element to the back of the container.
		\param p_Element The element to add, to the container.
		\return Returns nothing.
	*/
	bool PushBack(Type p_Element, unsigned int &p_ElementIndex = 0) {
		if(m_Size < m_Capacity || m_EmptySpace.size() > 0u) {
			if(m_EmptySpace.size() == 0u) {
				m_Data[m_Size] = p_Element;
				p_ElementIndex = m_Size;
				++m_Size;
			}
			else {
				m_Data[m_EmptySpace.front()] = p_Element;
				p_ElementIndex = m_EmptySpace.front();
				m_EmptySpace.pop_front();
			}
			++m_NumberOfElements;

			return true;
		}
		else {
			// Scale the vector's capacity.
			m_Capacity *= VECTOR_GROWTH_SCALAR;

			// Increase the capacity of the vector, so more elements can be added.
			ZoneNode *largerZoneNode = MemoryManagerInstance.ZoneAllocate(sizeof(Type) * m_Capacity);
			
			// Check there's a zone available. The pointer will be a nullptr, if there isn't a larger zone node available.
			if(largerZoneNode == nullptr)
				return false;

			// Create a temporary pointer, to the old zone node, so the data can be copied over.
			ZoneNode *oldZoneNode = m_MemoryNode;

			// Set the pointers to the new zone node.
			m_MemoryNode = largerZoneNode;
			m_Data = reinterpret_cast<Type *>(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock);

			// Copy all of the old data to the new location.
			std::memcpy(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock, oldZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock,
				(size_t)(oldZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock - oldZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock));

			// Inform the zone allocator that the old zone is no longer in use.
			MemoryManagerInstance.FreeZoneMemory(oldZoneNode);
		}

		if(m_Size < m_Capacity) {
			m_Data[m_Size] = p_Element;
			p_ElementIndex = m_Size;
			++m_Size;
			++m_NumberOfElements;

			return true;
		}
		return false;
	}

	/*!
		\brief Removes an element to the back of the container.
		\param p_Index The index of the element to remove.
		\return Returns nothing.
	*/
	void Remove(unsigned int p_Index) {
		if(m_Size > 0u && p_Index < m_Capacity) {
			// Set a char pointer to the start of the element that's going to be deleted.
			unsigned char *pointerToIndexElement = reinterpret_cast<unsigned char *>(m_Data + (sizeof(Type) * p_Index));
			// Zero the memory at that location.
			std::memset(&m_Data[p_Index], 0u, sizeof(Type));

			m_EmptySpace.push_back(p_Index);
			--m_NumberOfElements;
		}
	}

	/*!
		\brief Gets the total size of the vector.
		\return Returns the total size of the vector. 
	*/
	inline unsigned int Size() const {
		return m_Size;
	}
	/*!
		\brief Gets the  number of elements in the container.
		\return Returns the number of elements in the container.
	*/
	inline unsigned int NumberOfElements() const {
		return m_NumberOfElements;
	}

	/*!
		\brief Overloaded the index operator, so elements can be accessed easier.
		\param p_Index The index of the element to access.
		\return Returns the requested element.
	*/
	inline Type &operator[](unsigned int p_Index) {
		return *(m_Data + p_Index);
	}
};
}