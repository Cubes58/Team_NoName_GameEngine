#pragma once

#include "MemoryManager.h"

template <typename KeyType, typename ValueType>
struct HashNode {
	ValueType m_Value;
	KeyType m_Key;

	HashNode *m_LesserChild;
	HashNode *m_GreaterChild;

	HashNode(KeyType p_Key, ValueType p_value) 
		: m_Key(p_Key), m_Value(p_value) {  }
};

template <typename KeyType, typename ValueType>
class HashMap {
private:
	ZoneNode *m_MemoryNode = nullptr;
	HashNode<KeyType, ValueType> *m_StartNode;

	unsigned int m_Size;
	unsigned int m_Capacity;

	int HashFunction(KeyType p_Key) {
		return (int)(p_Key % 13u);
	}

public:
	HashMap(unsigned int p_Capacity = 150u) : m_Capacity(p_Capacity), m_Size(0u) {
		m_MemoryNode = MemoryManagerInstance.ZoneAllocate(sizeof(HashNode) * m_Capacity);
		m_StartNode = reinterpret_cast<HashNode<KeyType, ValueType>*>(m_MemoryNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock);
	}
	~HashMap() {
		MemoryManagerInstance.FreeZoneMemory(m_MemoryNode);
	}

	void Insert(HashNode<KeyType, ValueType> p_HashNode) {
		// Add the hash node.
		// Check to see if the map outgrows the capacity, if it does move it to a larger area, and double the capacity.
	}
	void Insert(KeyType p_Key, ValueType p_Value) {
		Insert(HashNode<KeyType, ValueType>(p_Key, p_Value));
	}

	void Remove(KeyType p_Key) {

	}
	void Remove(HashNode<KeyType, ValueType> p_HashNode) {
		Remove(p_HashNode.m_Key);
	}

	HashNode<KeyType, ValueType> *Get(KeyType p_Key) {



		return nullptr;
	}

	inline unsigned int Size() const {
		return m_Size;
	}
};