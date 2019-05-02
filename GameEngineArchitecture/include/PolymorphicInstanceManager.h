#pragma once

#include "HashMap.h"
#include "Vector.h"

#include "AABBComponent.h"
#include "AttackComponent.h"
#include "BodyComponent.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "ModelComponent.h"
#include "MovementComponent.h"
#include "TransformComponent.h"

class PolymorphicInstanceManager {
private:
	const constexpr static unsigned int s_m_ComponentContainersCapacity = 200;

	PolymorphicInstanceManager() : m_ModelComponents(s_m_ComponentContainersCapacity), m_AABBComponents(s_m_ComponentContainersCapacity), m_AttackComponents(s_m_ComponentContainersCapacity),
		m_BodyComponents(s_m_ComponentContainersCapacity), m_CameraComponents(s_m_ComponentContainersCapacity), m_HealthComponents(s_m_ComponentContainersCapacity),
		m_MovementComponents(s_m_ComponentContainersCapacity), m_TransformComponents(s_m_ComponentContainersCapacity)  {  }
	~PolymorphicInstanceManager() = default;

public:
	CC::Vector<ModelComponent> m_ModelComponents;
	CC::Vector<AABBComponent> m_AABBComponents;
	CC::Vector<AttackComponent> m_AttackComponents;
	CC::Vector<BodyComponent> m_BodyComponents;
	CC::Vector<CameraComponent> m_CameraComponents;
	CC::Vector<HealthComponent> m_HealthComponents;
	CC::Vector<MovementComponent> m_MovementComponents;
	CC::Vector<TransformComponent> m_TransformComponents;

	/*!
		\brief Singleton instance.
		\return Returns the singletone instance, of this class.
	*/
	static PolymorphicInstanceManager &Instance();
};