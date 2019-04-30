#pragma once

#include <functional>

#include "MemoryManager.h"

namespace CC {
template <typename KeyType, typename ValueType>
struct HashNode {
	ValueType m_Value;
	KeyType m_Key;

	HashNode *m_Left;
	HashNode *m_Right;

	HashNode() = default;
	HashNode(KeyType p_Key, ValueType p_value)
		: m_Key(p_Key), m_Value(p_value) {
	}
	HashNode(KeyType p_Key, ValueType p_value, HashNode *p_Left, HashNode *p_Right)
		: m_Key(p_Key), m_Value(p_value), m_Left(p_Left), m_Right(p_Right) {
	}
};

template <typename KeyType, typename ValueType>
class HashMap {
private:
	std::list<unsigned int> m_EmptySpace;	//!< A list of spaces that are free, within the allocated block of memory. 
	ZoneNode *m_MemoryNode = nullptr;
	HashNode<KeyType, ValueType> *m_StartNode;

	unsigned int m_Size;
	unsigned int m_Capacity;

	inline unsigned int HashFunction(KeyType p_Key) const {
		return std::hash<KeyType>{}(p_Key);
	}

public:
	HashMap(unsigned int p_Capacity = 150u) : m_Capacity(p_Capacity), m_Size(0u) {
		m_MemoryNode = MemoryManagerInstance.ZoneAllocate(sizeof(HashNode<KeyType, ValueType>) * m_Capacity);
		m_StartNode = reinterpret_cast<HashNode<KeyType, ValueType>*>(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock);
	}
	~HashMap() {
		MemoryManagerInstance.FreeZoneMemory(m_MemoryNode);
	}

	void Insert(HashNode<KeyType, ValueType> p_HashNode) {
		unsigned int hashValue = HashFunction(p_HashNode.m_Key);

		bool b = true;

	}
	void Insert(KeyType p_Key, ValueType p_Value) {
		Insert(HashNode<KeyType, ValueType>(p_Key, p_Value));
	}

	void Remove(KeyType p_Key) {
		unsigned int hashValue = HashFunction(p_Key);



	}
	void Remove(HashNode<KeyType, ValueType> p_HashNode) {
		Remove(p_HashNode.m_Key);
	}

	HashNode<KeyType, ValueType> *Get(KeyType p_Key) {
		unsigned int hashValue = HashFunction(p_Key);



		return nullptr;
	}

	inline unsigned int Size() const {
		return m_Size;
	}
};
}