#include "MemoryManager.h"

#include <cstdlib>
#include <string>

MemoryManager::MemoryManager() : m_DoubleEndedStackAllocator(STACK_SIZE_IN_MEGABYTES * NUMBER_OF_BYTES_PER_MEGABYTE),
m_ZoneAllocator(m_DoubleEndedStackAllocator.Allocate(ZONE_SIZE_IN_MEGABYTES * NUMBER_OF_BYTES_PER_MEGABYTE, true)) {
}

MemoryManager::~MemoryManager() {
	Clear();
}

MemoryManager &MemoryManager::Instance() {
	static MemoryManager s_Instance;

	return s_Instance;
}

MemoryBlock MemoryManager::StackAllocate(unsigned int p_NumberOfBytes, bool p_Static) {
	return m_DoubleEndedStackAllocator.Allocate(p_NumberOfBytes, p_Static);
}

ZoneNode *MemoryManager::ZoneAllocate(unsigned int p_NumberOfBytes) {
	return m_ZoneAllocator.Allocate(p_NumberOfBytes);
}

void MemoryManager::FreeStackMemory(Marker p_Marker) {
	m_DoubleEndedStackAllocator.FreeMemory(p_Marker);
}

void MemoryManager::FreeZoneMemory(ZoneNode *p_ZoneNode) {
	m_ZoneAllocator.FreeMemory(p_ZoneNode);
}

void MemoryManager::Clear() {
	m_DoubleEndedStackAllocator.~DoubleEndedStackAllocator();
}