/**
@file HashMap.h
@brief Custom implementation of a hash map, which uses the double ended stack allocator.
*/
#pragma once

#include "MemoryManager.h"

#define HASH_MAP_GROWTH_SCALAR 2u	//!< The hash map's capacity growth scalar, when the hash map needs to find more space, to fit more elements in.

//! Custom Container.
namespace CC {
/**
	\brief A structure to represent a hash node.
*/
template <typename KeyType, typename ValueType>
struct HashNode {
	ValueType m_Value;	//!< The data stored in the hash node.
	KeyType m_Key;		//!< The hash node's key.

	HashNode *m_Left;	//!< The left child, of the hash node.
	HashNode *m_Right;	//!< The right child, of the hash node.

	/*!
		\brief Constructor.
	*/
	HashNode() = default;
	/*!
		\brief Constructor.
		\param p_Key The Key used to identify the hash node.
		\param p_value The data stored in the hash node.
	*/
	HashNode(KeyType p_Key, ValueType p_value)
		: m_Key(p_Key), m_Value(p_value) {
	}
	/*!
		\brief Constructor.
		\param p_Key The Key used to identify the hash node.
		\param p_value The data stored in the hash node.
		\param p_Left A pointer to the left child, of the hash node.
		\param p_Right A pointer to the right child, of the hash node.
	*/
	HashNode(KeyType p_Key, ValueType p_value, HashNode *p_Left, HashNode *p_Right)
		: m_Key(p_Key), m_Value(p_value), m_Left(p_Left), m_Right(p_Right) {
	}

	/*!
		\brief Assigns the values from one hash node to this hash node.
		\param p_Other The hash node to assign the values from.
		\return Returns a reference to this object.
	*/
	HashNode<KeyType, ValueType> &operator=(const HashNode<KeyType, ValueType> &p_Other) {
		m_Value = p_Other.m_Value;
		m_Key = p_Other.m_Key;
		m_Left = p_Other.m_Left;
		m_Right = p_Other.m_Right;

		return *this;
	}
};

/*! \class HashMap
	\brief Manages a large chunk of memory, which stores elements .
*/

template <typename KeyType, typename ValueType>
class HashMap {
private:
	std::list<unsigned int> m_EmptySpace;	//!< A list of spaces that are free, within the allocated block of memory. 
	ZoneNode *m_MemoryNode = nullptr;	//!< A pointer to the zone node, used to encapsulate the data.
	HashNode<KeyType, ValueType> *m_Data = nullptr;	//!< A pointer to the start of the data.
	HashNode<KeyType, ValueType> *m_RootNode = nullptr;	//!< A pointer to the root node.

	unsigned int m_Size;		// The total size of the hash map.
	unsigned int m_Capacity;	// The maximum capacity of the hash map, before it needs to allocate more memory.

	/*!
		\brief Sets the hash node as the child of the necessary hash node.
		\param p_HashNode The hash node to set as the child of another hash node.
		\return Nothing.
	*/
	void SetNodePosition(HashNode<KeyType, ValueType> &p_HashNode) {
		HashNode<KeyType, ValueType> *currentNode = m_RootNode;
		KeyType key = p_HashNode.m_Key;

		HashNode<KeyType, ValueType> *previousNode = currentNode;
		// Find where the node will be placed.
		bool isLeftChild = false;
		while(currentNode != nullptr) {
			previousNode = currentNode;
			if(key < currentNode->m_Key) {
				currentNode = currentNode->m_Left;
				isLeftChild = true;
			}
			else {
				currentNode = currentNode->m_Right;
				isLeftChild = false;
			}
		}
		// Place the node.
		if(isLeftChild) {
			if(m_EmptySpace.size() > 0u)
				previousNode->m_Left = &m_Data[m_EmptySpace.front()];
			else
				previousNode->m_Left = &m_Data[m_Size];
		}
		else {
			if(m_EmptySpace.size() > 0u)
				previousNode->m_Left = &m_Data[m_EmptySpace.front()];
			else
				previousNode->m_Right = &m_Data[m_Size];
		}

		// Set the children to nullptrs.
		p_HashNode.m_Left = nullptr;
		p_HashNode.m_Right = nullptr;
	}

public:
	/*!
		\brief Constructor.
		\param p_Capacity Number of elements/hash nodes the reserved memory can fit within it.
	*/
	HashMap(unsigned int p_Capacity = 150u) : m_Capacity(p_Capacity), m_Size(0u) {
		m_MemoryNode = MemoryManagerInstance.ZoneAllocate(sizeof(HashNode<KeyType, ValueType>) * m_Capacity);
		m_Data = reinterpret_cast<HashNode<KeyType, ValueType>*>(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock);
	}
	/*!
		\brief Destructor.
	*/
	~HashMap() {
		MemoryManagerInstance.FreeZoneMemory(m_MemoryNode);
	}

	/*!
		\brief Inserts a hash node into the hash map.
		\param p_HashNode The hash node to add, to the hash map.
		\return Returns true if the hash node was successfully added, otherwise false.
	*/
	bool Insert(HashNode<KeyType, ValueType> p_HashNode) {
		if(m_RootNode == nullptr) {
			p_HashNode.m_Left = nullptr;
			p_HashNode.m_Right = nullptr;

			m_Data[m_Size] = p_HashNode;
			m_RootNode = &m_Data[m_Size];
			++m_Size;
			return true;
		}

		SetNodePosition(p_HashNode);
		if(m_Size < m_Capacity || m_EmptySpace.size() > 0u) {
			if(m_EmptySpace.size() == 0u) {
				m_Data[m_Size] = p_HashNode;

				++m_Size;
			}
			else {
				m_Data[m_EmptySpace.front()] = p_HashNode;
				m_EmptySpace.pop_front();
			}
			return true;
		}
		else {
			// Scale the hash maps's capacity.
			m_Capacity *= HASH_MAP_GROWTH_SCALAR;

			// Increase the capacity of the vector, so more elements can be added.
			ZoneNode *largerZoneNode = MemoryManagerInstance.ZoneAllocate(sizeof(HashNode<KeyType, ValueType>) * m_Capacity);

			// Check there's a zone available. The pointer will be a nullptr, if there isn't a larger zone node available.
			if(largerZoneNode == nullptr)
				return false;

			// Create a temporary pointer, to the old zone node, so the data can be copied over.
			ZoneNode *oldZoneNode = m_MemoryNode;

			// Set the pointers to the new zone node.
			m_MemoryNode = largerZoneNode;
			m_Data = reinterpret_cast<HashNode<KeyType, ValueType> *>(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock);
			m_RootNode = &m_Data[0];

			// Copy all of the old data to the new location.
			std::memcpy(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock, oldZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock,
				(size_t)(oldZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock - oldZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock));

			// Inform the zone allocator that the old zone is no longer in use.
			MemoryManagerInstance.FreeZoneMemory(oldZoneNode);
		}

		SetNodePosition(p_HashNode);
		if(m_Size < m_Capacity) {
			m_Data[m_Size] = p_HashNode;
			++m_Size;
			return true;
		}
		return false;
	}
	/*!
		\brief Inserts a hash node into the hash map.
		\param p_Key The key of the hash node, to add to the hash map.
		\param p_Value The data stored within the hash node, to add to the hash map.
		\return Returns true if the hash node was successfully added, otherwise false.
	*/
	void Insert(KeyType p_Key, ValueType p_Value) {
		Insert(HashNode<KeyType, ValueType>(p_Key, p_Value));
	}

	/*!
		\brief Removes a hash node from the hash map.
		\param p_Key The key of the hash node, to remove from the hash map.
		\return Nothing.
	*/
	void Remove(KeyType p_Key) {
		HashNode<KeyType, ValueType> *nodeToRemove = m_RootNode;
		HashNode<KeyType, ValueType> *parentNodeOfTheOneToRemove = nodeToRemove;

		// Find where the node is.
		bool isLeftChild = false;
		while(nodeToRemove != nullptr) {
			parentNodeOfTheOneToRemove = nodeToRemove;

			if(p_Key < nodeToRemove->m_Key) {
				nodeToRemove = nodeToRemove->m_Left;
				isLeftChild = true;
			}
			else {
				nodeToRemove = nodeToRemove->m_Right;
				isLeftChild = false;
			}

			if(p_Key == nodeToRemove->m_Key)
				break;
		}
		// Store temporary pointers to the child node[s].
		HashNode<KeyType, ValueType> *leftChildNode = nodeToRemove->m_Left;
		HashNode<KeyType, ValueType> *rightChildNode = nodeToRemove->m_Right;

		// Find where the node is, within the memory block.
		unsigned int nodeHashIndex = 0;
		do {
			if(m_Data[nodeHashIndex].m_Key == nodeToRemove->m_Key)
				break;

			++nodeHashIndex;
		} while(nodeHashIndex < m_Capacity && m_Data[nodeHashIndex].m_Key != p_Key);

		// Add the index to the empty space list, so it can be filled by another hash node.
		m_EmptySpace.push_back(nodeHashIndex);

		// Amend the hash node pointers.
		if(isLeftChild)
			parentNodeOfTheOneToRemove->m_Left = leftChildNode;
		else
			parentNodeOfTheOneToRemove->m_Right = leftChildNode;

		// Fix the broken leaf.


		// Remove the data of the hash node requested.
		std::memset(&m_Data[nodeHashIndex], 0, sizeof(HashNode<KeyType, ValueType>));
	}
	/*!
		\brief Removes a hash node from the hash map.
		\param p_HashNode The hash node, to remove from the hash map.
		\return Nothing.
	*/
	void Remove(HashNode<KeyType, ValueType> p_HashNode) {
		Remove(p_HashNode.m_Key);
	}

	/*!
		\brief Finds, and returns an element in the hash map.
		\param p_Key The key of the element/hash node to find.
		\return Returns the found element/hash node, if it finds it, otherwise a nullptr.
	*/
	HashNode<KeyType, ValueType> *Get(KeyType p_Key) {
		HashNode<KeyType, ValueType> *currentNode = m_RootNode;

		while(currentNode != nullptr) {
			if(currentNode->m_Key == p_Key)
				return currentNode;

			if(p_Key < currentNode->m_Key)
				currentNode = currentNode->m_Left;
			else
				currentNode = currentNode->m_Right;
		}

		return nullptr;
	}

	/*!
		\brief Gets the total size of the hash map.
		\return Returns the total size of the hash map.
	*/
	inline unsigned int Size() const {
		return m_Size;
	}
};
}