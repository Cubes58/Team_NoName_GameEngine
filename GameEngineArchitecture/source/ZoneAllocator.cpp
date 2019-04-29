#include "ZoneAllocator.h"

#include <string>

#include "MemoryManager.h"

ZoneAllocator::ZoneAllocator(MemoryBlock p_MemoryBlock)
	: m_MemoryBlock(p_MemoryBlock) {

	// Single free (node) block, which is the whole zone memory section.
	m_StartNode = reinterpret_cast<ZoneNode*>(m_MemoryBlock.m_StartPointer);
	m_StartNode->SetInformation(MemoryHeaderInformation(m_MemoryBlock.m_StartPointer, m_MemoryBlock.m_Size - sizeof(MemoryHeaderInformation), false), nullptr, nullptr);
}

ZoneNode *ZoneAllocator::Allocate(unsigned int p_NumberOfBytes) {
	p_NumberOfBytes += sizeof(ZoneNode);	// Every node must have additional management information.

	ZoneNode *newNode = nullptr;
	ZoneNode *currentNode = m_StartNode;
	while(currentNode != nullptr) {
		if(!currentNode->m_ManagedMemoryBlock.m_Occupied && currentNode->m_ManagedMemoryBlock.m_SizeOfManagedData >= p_NumberOfBytes) {
			currentNode->m_ManagedMemoryBlock.m_SizeOfManagedData -= p_NumberOfBytes;	// Remove size, from the stored size.
			currentNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock -= p_NumberOfBytes;	// Remove the memory, from the memory block.

			// Copy the unoccupied node's data to a new location, using a temp pointer to keep track of where it's placed.
			ZoneNode *tempNode = reinterpret_cast<ZoneNode*>(currentNode->m_ManagedMemoryBlock.m_StartOfHeader + p_NumberOfBytes);
			p_NumberOfBytes -= sizeof(ZoneNode);
			tempNode->SetInformation(MemoryHeaderInformation(reinterpret_cast<Marker>(tempNode), currentNode->m_ManagedMemoryBlock.m_SizeOfManagedData - p_NumberOfBytes, false), currentNode->m_PreviousNode, currentNode->m_NextNode);

			// Keep track of the previous zone node.
			ZoneNode *previousNode = currentNode->m_PreviousNode;

			// Set the new zone node's information.
			newNode = reinterpret_cast<ZoneNode*>(currentNode->m_ManagedMemoryBlock.m_StartOfHeader);
			newNode->SetInformation(MemoryHeaderInformation(reinterpret_cast<Marker>(newNode), p_NumberOfBytes, true));

			// Amend the current node data.
			currentNode = tempNode;
			currentNode->SetInformation(tempNode->m_ManagedMemoryBlock, tempNode->m_PreviousNode, tempNode->m_NextNode);

			// Set the zone node behind the current zone node, and infront of the previous node, of the current node. 
			if(previousNode != nullptr)
				previousNode->m_NextNode = newNode;
			newNode->m_PreviousNode = previousNode;

			newNode->m_NextNode = currentNode;
			currentNode->m_PreviousNode = newNode;
			
			// Set the new node to the start node.
			m_StartNode = newNode;

			return newNode;
		}
		else
			currentNode = currentNode->m_NextNode;
	}

	currentNode = m_StartNode->m_PreviousNode;
	while(currentNode != nullptr) {
		if(!currentNode->m_ManagedMemoryBlock.m_Occupied && currentNode->m_ManagedMemoryBlock.m_SizeOfManagedData >= p_NumberOfBytes) {
			currentNode->m_ManagedMemoryBlock.m_SizeOfManagedData -= p_NumberOfBytes;
			currentNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock -= p_NumberOfBytes;

			// Copy the unoccupied node's data to a new location, using a temp pointer to keep track of where it's placed.
			ZoneNode *tempNode = reinterpret_cast<ZoneNode*>(currentNode->m_ManagedMemoryBlock.m_StartOfHeader + p_NumberOfBytes);
			p_NumberOfBytes -= sizeof(ZoneNode);
			tempNode->SetInformation(MemoryHeaderInformation(reinterpret_cast<Marker>(tempNode), currentNode->m_ManagedMemoryBlock.m_SizeOfManagedData - p_NumberOfBytes, false), currentNode->m_PreviousNode, currentNode->m_NextNode);

			// Keep track of the previous zone node.
			ZoneNode *previousNode = currentNode->m_PreviousNode;

			// Set the new zone node's information.
			newNode = reinterpret_cast<ZoneNode*>(currentNode->m_ManagedMemoryBlock.m_StartOfHeader);
			newNode->SetInformation(MemoryHeaderInformation(reinterpret_cast<Marker>(newNode), p_NumberOfBytes, true));

			// Amend the current node data.
			currentNode = tempNode;
			currentNode->SetInformation(tempNode->m_ManagedMemoryBlock, tempNode->m_PreviousNode, tempNode->m_NextNode);

			// Set the zone node behind the current zone node, and infront of the previous node, of the current node. 
			if(previousNode != nullptr)
				previousNode->m_NextNode = newNode;
			newNode->m_PreviousNode = previousNode;

			newNode->m_NextNode = currentNode;
			currentNode->m_PreviousNode = newNode;

			// Set the new node to the start node.
			m_StartNode = newNode;

			return newNode;
		}
		else
			currentNode = currentNode->m_PreviousNode;
	}

	return nullptr;
}

void ZoneAllocator::FreeMemory(ZoneNode *p_ZoneNode) {
	ZoneNode *nextNode = p_ZoneNode->m_NextNode;
	ZoneNode *previousNode = p_ZoneNode->m_PreviousNode;

	// If all three node zones connect then the connecting ones will need to be known.
	// The previous previous zone won't be unoccupied because otherwise it would've been merged with the previous zone, already, same for the next zone node.
	// It'll either be a nullptr, or an occupied zone node.
	ZoneNode *nextNextNode = nullptr;
	if (nextNode != nullptr)
		nextNode->m_NextNode;
	ZoneNode *previousPreviousNode = nullptr;
	if(previousNode != nullptr)
		previousPreviousNode = previousNode->m_PreviousNode;

	// Check if a zone node is occupied.
	auto isZoneNodeOccupied = [](ZoneNode *p_ZoneNode)->bool {
		if(p_ZoneNode != nullptr) {
			if(p_ZoneNode->m_ManagedMemoryBlock.m_Occupied)
				return true;
			else 
				return false;
		}
		return true;
	};

	// Returns true if p_ZoneNodeOne has a smaller memory address than p_ZoneNodeTwo, otherwise false.
	auto compareMemoryZoneStartAddress = [](ZoneNode *p_ZoneNodeOne, ZoneNode *p_ZoneNodeTwo)->bool {
		if(p_ZoneNodeOne->m_ManagedMemoryBlock.m_StartOfMemoryBlock < p_ZoneNodeTwo->m_ManagedMemoryBlock.m_StartOfMemoryBlock)
			return true;
		return false;
	};

	ZoneNode *newFreeZoneNode = nullptr;
	// Check, to see if the unoccupied node zone can be merged with other surroundings zone nodes.
	if(!isZoneNodeOccupied(nextNode) && !isZoneNodeOccupied(previousNode)) {
		// Merge all three zones together.
		// Find which node to add the memory header information to. (Should always be previous node, but it doesn't hurt to check.)
		if(compareMemoryZoneStartAddress(previousNode, p_ZoneNode) && compareMemoryZoneStartAddress(previousNode, nextNode)) {
			// Set the header information at the start of the previous zone node.
			newFreeZoneNode = previousNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + nextNode->m_ManagedMemoryBlock.m_SizeOfManagedData + (sizeof(ZoneNode) * 2u));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + nextNode->m_ManagedMemoryBlock.m_SizeOfManagedData + (sizeof(ZoneNode) * 2u));
		}
		else if(compareMemoryZoneStartAddress(p_ZoneNode, previousNode) && compareMemoryZoneStartAddress(p_ZoneNode, nextNode)) {
			// Set the header information at the start of the p_ZoneNode, node.
			newFreeZoneNode = p_ZoneNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (previousNode->m_ManagedMemoryBlock.m_SizeOfManagedData + nextNode->m_ManagedMemoryBlock.m_SizeOfManagedData + (sizeof(ZoneNode) * 2u));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (previousNode->m_ManagedMemoryBlock.m_SizeOfManagedData + nextNode->m_ManagedMemoryBlock.m_SizeOfManagedData + (sizeof(ZoneNode) * 2u));
		}
		else {
			// Set the header information at the start of the next zone node.
			newFreeZoneNode = nextNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (previousNode->m_ManagedMemoryBlock.m_SizeOfManagedData + p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + (sizeof(ZoneNode) * 2u));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (previousNode->m_ManagedMemoryBlock.m_SizeOfManagedData + p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + (sizeof(ZoneNode) * 2u));
		}
		// Modify the memory header information.
		newFreeZoneNode->m_ManagedMemoryBlock.m_Occupied = false;

		// Amend the next and previous node pointers.
		newFreeZoneNode->m_PreviousNode = previousPreviousNode;
		newFreeZoneNode->m_NextNode = nextNextNode;

		// Ensure they're valid Zone nodes.
		if(previousPreviousNode != nullptr)
			previousPreviousNode->m_NextNode = newFreeZoneNode;
		if(nextNextNode != nullptr)
			nextNextNode->m_PreviousNode = newFreeZoneNode;

		// Zero the data.
		//std::memset(newFreeZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock, 0, newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData);

		// Ensure the start node is a valid node, even after nodes have been merged/wiped.
		m_StartNode = newFreeZoneNode;
	}
	else if(!isZoneNodeOccupied(nextNode)) {
		// Merge this zone with the next one.
		// Find which node to add the memory header information to.
		if(compareMemoryZoneStartAddress(nextNode, p_ZoneNode)) {
			// Set the header information at the start of the next zone node.
			newFreeZoneNode = nextNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
		}
		else {
			// Set the header information at the start of the p_ZoneNode, node.
			newFreeZoneNode = p_ZoneNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (nextNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (nextNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
		}
		// Modify the memory header information.
		newFreeZoneNode->m_ManagedMemoryBlock.m_Occupied = false;

		// Amend the node pointers.
		newFreeZoneNode->m_NextNode = nextNextNode;
		if (nextNextNode != nullptr)
			nextNextNode->m_PreviousNode = newFreeZoneNode;

		newFreeZoneNode->m_PreviousNode = previousNode;
		if(nextNode != nullptr)
			previousNode->m_NextNode = newFreeZoneNode;

		// Zero the data.
		std::memset(newFreeZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock, 0, newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData);

		// Ensure the start node is a valid node, even after nodes have been merged/wiped.
		m_StartNode = newFreeZoneNode;
	}
	else if(!isZoneNodeOccupied(previousNode)) {
		// Merge this zone with the previous one.
		// Find which node to add the memory header information to.
		if(compareMemoryZoneStartAddress(previousNode, p_ZoneNode)) {
			// Set the header information at the start of the next zone node.
			newFreeZoneNode = previousNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
		}
		else {
			// Set the header information at the start of the p_ZoneNode, node.
			newFreeZoneNode = p_ZoneNode;
			newFreeZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock += (previousNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
			newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData += (previousNode->m_ManagedMemoryBlock.m_SizeOfManagedData + sizeof(ZoneNode));
		}
		// Modify the memory header information.
		newFreeZoneNode->m_ManagedMemoryBlock.m_Occupied = false;

		// Amend the node pointers.
		newFreeZoneNode->m_PreviousNode = previousPreviousNode;
		if(previousPreviousNode != nullptr)
			previousPreviousNode->m_NextNode = newFreeZoneNode;

		newFreeZoneNode->m_NextNode = nextNode;
		if(nextNode != nullptr)
			nextNode->m_PreviousNode = newFreeZoneNode;

		// Zero the data.
		std::memset(newFreeZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock, 0, newFreeZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData);

		// Ensure the start node is a valid node, even after nodes have been merged/wiped.
		m_StartNode = newFreeZoneNode;
	}
	else {
		// It's its own block of unoccupied memory.
		// Reset some of the header information (stating it's not occupied).
		p_ZoneNode->m_ManagedMemoryBlock.m_Occupied = false;

		unsigned int size = p_ZoneNode->m_ManagedMemoryBlock.m_EndOfMemoryBlock - p_ZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock;

		// Zero the data.
		std::memset(p_ZoneNode->m_ManagedMemoryBlock.m_StartOfMemoryBlock, 0, p_ZoneNode->m_ManagedMemoryBlock.m_SizeOfManagedData);

		// Ensure the start node is a valid node, even after nodes have been merged/wiped.
		m_StartNode = p_ZoneNode;
	}
}