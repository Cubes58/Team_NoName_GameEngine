#include "PolymorphicInstanceManager.h"

PolymorphicInstanceManager & PolymorphicInstanceManager::Instance() {
	static PolymorphicInstanceManager s_PolymorphicInstanceManager;

	return s_PolymorphicInstanceManager;
}