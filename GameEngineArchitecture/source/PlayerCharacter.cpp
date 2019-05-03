#include "PlayerCharacter.h"

#include "PolymorphicInstanceManager.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "AABBComponent.h"
#include "BodyComponent.h"

PlayerCharacter::PlayerCharacter(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, const glm::vec3 &p_AABBSize, float p_Health, float p_PlayerVelocity, float p_PlayerRotationSpeed)
	: m_CameraState(CameraViewState::FIRST_PERSON_CAMERA), m_PlayerMovementSpeed(p_PlayerVelocity), m_PlayerRotationSpeed(p_PlayerRotationSpeed), m_OriginalPosition(p_Position), m_OriginalOrientation(p_Orientation) {

	unsigned int index = 0;
	ModelComponent modelComponent(p_ModelName);
	PolymorphicInstanceManager::Instance().m_ModelComponents.PushBack(modelComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_ModelComponents.At(index));

	AABBComponent AABBComponent(p_Position, p_AABBSize);
	PolymorphicInstanceManager::Instance().m_AABBComponents.PushBack(AABBComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_AABBComponents.At(index));

	TransformComponent transformComponent(p_Position, p_Orientation, p_Scale);
	PolymorphicInstanceManager::Instance().m_TransformComponents.PushBack(transformComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_TransformComponents.At(index));

	CameraComponent cameraComponent(p_Position, p_Orientation);
	PolymorphicInstanceManager::Instance().m_CameraComponents.PushBack(cameraComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_CameraComponents.At(index));

	HealthComponent healthComponent(p_Health);
	PolymorphicInstanceManager::Instance().m_HealthComponents.PushBack(healthComponent, index);
	AddComponent(PolymorphicInstanceManager::Instance().m_HealthComponents.At(index));
}

void PlayerCharacter::OnUpdate(float p_DeltaTime) {
	// Get the transform component details.
	TransformComponent *transform = GetComponent<TransformComponent>();
	transform->Translate(m_TranslationVector * p_DeltaTime * inverse(transform->m_Orientation));
	transform->Yaw(m_RotationValue * p_DeltaTime);
	GetComponent<AABBComponent>()->SetPosition(transform->m_Position);
	glm::quat inverseOrientation = inverse(transform->m_Orientation);

	switch (m_CameraState) {
	case CameraViewState::FIRST_PERSON_CAMERA:
		// Place camera on the player, for first person.
		GetComponent<CameraComponent>()->m_Position = -1.0f * (transform->m_Position + glm::vec3(0.0f, 2.0f, -0.35f) * inverseOrientation);
		break;
	case CameraViewState::THIRD_PERSON_CAMERA:
		// Third person view - stay at a set distance from the player.
		GetComponent<CameraComponent>()->m_Position = -1.0f * (transform->m_Position + glm::vec3(0.0f, 2.0f, 7.5f) * inverseOrientation);
		break;
	}
	GetComponent<CameraComponent>()->m_Orientation = inverseOrientation;

	HealthComponent *health = GetComponent<HealthComponent>();
	//health->OnUpdate(p_DeltaTime);
	if (health->IsHealthBelowZero()) {
		OnMessage("Reset");
	}
}

void PlayerCharacter::OnMessage(const std::string &p_Message) {
	if (p_Message == "SetCameraFirstPerson") {
		m_CameraState = CameraViewState::FIRST_PERSON_CAMERA;
	}
	else if (p_Message == "SetCameraThirdPerson") {
		m_CameraState = CameraViewState::THIRD_PERSON_CAMERA;
	}
	else if (p_Message.compare(0, 12, "RotateCamera") == 0) {
		TransformComponent *transform = GetComponent<TransformComponent>();
		m_RotationValue = 0;
		if (p_Message == "RotateCameraLeft")
			m_RotationValue = m_PlayerRotationSpeed;
		else if (p_Message == "RotateCameraRight")
			m_RotationValue = -m_PlayerRotationSpeed;

		// Inverse the player rotation.
		if (m_InverseRotation) 
			m_RotationValue *= -1;

		//transform->Yaw(m_RotationValue);
	}
	else if (p_Message.compare(0, 10, "MovePlayer") == 0) {
		// Determine the correct translation vector.
		if (p_Message == "MovePlayerForward")
			m_TranslationVector.z = -m_PlayerMovementSpeed;
		else if (p_Message == "MovePlayerBackward")
			m_TranslationVector.z = m_PlayerMovementSpeed;
		else if (p_Message == "MovePlayerLeft")
			m_TranslationVector.x = -m_PlayerMovementSpeed;
		else if (p_Message == "MovePlayerRight")
			m_TranslationVector.x = m_PlayerMovementSpeed;

		//std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
		//transform->Translate(m_TranslationVector * inverse(transform->m_Orientation));
	}
	else if (p_Message.compare(0, 10, "StopPlayer") == 0) {
		if (p_Message == "StopPlayerRight")
			m_TranslationVector.x = 0;
		else if (p_Message == "StopPlayerLeft")
			m_TranslationVector.x = 0;
		else if (p_Message == "StopPlayerForward")
			m_TranslationVector.z = 0;
		else if (p_Message == "StopPlayerBackward")
			m_TranslationVector.z = 0;
	}
	else if (p_Message == "HitByEnemyTower") {
		GetComponent<HealthComponent>()->OnMessage("HitByEnemyTower");
	}
	else if (p_Message == "NoInvertCameraRotation") {
		m_InverseRotation = false;
	}
	else if (p_Message == "InvertCameraRotation") {
		m_InverseRotation = true;
	}
	else if (p_Message == "Reset") {
		GetComponent<HealthComponent>()->OnMessage("Reset");
		GetComponent<TransformComponent>()->m_Position = m_OriginalPosition;
		GetComponent<TransformComponent>()->m_Orientation = m_OriginalOrientation;
	}

	if(p_Message == "StopRotateCameraRight") {
		m_RotationValue = 0;
	}
	if(p_Message == "StopRotateCameraRight") {
		m_RotationValue = 0;
	}
}

void PlayerCharacter::HandleCursorInput(double p_DeltaMouseXPosition, double p_DeltaMouseYPosition) {
	//GetComponent<TransformComponent>()->Pitch(p_DeltaMouseYPosition);
	//GetComponent<TransformComponent>()->Yaw(p_DeltaMouseXPosition);
	
	
	double rotationAngleLeftRight = glm::radians(p_DeltaMouseXPosition * m_MouseSensitivity);

	// Rotate around the Y axis - player can rotate left/right, to look around.
	GetComponent<TransformComponent>()->Rotate((float)((m_InverseRotation) ? (rotationAngleLeftRight) : (rotationAngleLeftRight  * -1.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
	
}