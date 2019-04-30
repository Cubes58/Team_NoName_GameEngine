#pragma once

#include "MemoryBlock.h"

template <typename NodeType>
struct ListNode {
	NodeType *m_Data;

	ListNode *m_PreviousNode;
	ListNode *m_NextNode;

	ListNode() = default;
	ListNode(NodeType *p_Data, ListNode *p_PreviousNode = nullptr, ListNode *p_NextNode = nullptr)
		: m_Data(p_Data), m_PreviousNode(p_PreviousNode), m_NextNode(p_NextNode) { }
};

class ZoneAllocator;
class MemoryManager;

template <typename ListType>
class DoublyLinkedList {
private:
	friend ZoneAllocator;
	friend MemoryManager;

	ListNode<ListType> *m_StartNode = nullptr;
	MemoryBlock m_MemoryBlock;

public:
	DoublyLinkedList(unsigned int p_Capacity = 50u);
	~DoublyLinkedList();

	void InsertBeginning(ListType p_ListNode);
	void InsertAfter(ListType p_ListNode);
	void InsertEnd(ListType p_ListNode);

	void Delete(ListType p_ListNode);
};