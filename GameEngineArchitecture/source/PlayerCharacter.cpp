#include "PlayerCharacter.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "HealthComponent.h"

PlayerCharacter::PlayerCharacter(const std::string &p_ModelName, const glm::vec3 &p_Position, const glm::quat &p_Orientation, const glm::vec3 &p_Scale, float p_Health, float p_PlayerVelocity, float p_PlayerRotationSpeed)
	: m_CameraState(CameraViewState::FIRST_PERSON_CAMERA), m_PlayerMovementSpeed(p_PlayerVelocity), m_PlayerRotationSpeed(p_PlayerRotationSpeed), m_OriginalPosition(p_Position), m_OriginalOrientation(p_Orientation) {
	AddComponent(std::make_shared<ModelComponent>(p_ModelName));
	AddComponent(std::make_shared<TransformComponent>(p_Position, p_Orientation, p_Scale));
	AddComponent(std::make_shared<CameraComponent>(p_Position, p_Orientation));
	AddComponent(std::make_shared<HealthComponent>(p_Health));
}

void PlayerCharacter::OnUpdate(float p_DeltaTime) {
	// Get the transform component details.
	std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
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

	std::shared_ptr<HealthComponent> health = GetComponent<HealthComponent>();
	health->OnUpdate(p_DeltaTime);
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
		std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
		float rotationValue;
		if (p_Message == "RotateCameraLeft")
			rotationValue = m_PlayerRotationSpeed;
		else
			rotationValue = -m_PlayerRotationSpeed;

		// Inverse the player rotation.
		if (m_InverseRotation) 
			rotationValue *= -1;

		transform->Yaw(rotationValue);
	}
	else if (p_Message.compare(0, 10, "MovePlayer") == 0) {
		// Determine the correct translation vector.
		glm::vec3 translationVector(0.0f, 0.0f, 0.0f);
		if (p_Message == "MovePlayerForward")
			translationVector.z = -m_PlayerMovementSpeed;
		else if (p_Message == "MovePlayerBackward")
			translationVector.z = m_PlayerMovementSpeed;
		else if (p_Message == "MovePlayerLeft")
			translationVector.x = -m_PlayerMovementSpeed;
		else if (p_Message == "MovePlayerRight")
			translationVector.x = m_PlayerMovementSpeed;

		std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
		transform->Translate(translationVector * inverse(transform->m_Orientation));
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
}

void PlayerCharacter::HandleCursorInput(double p_DeltaMouseXPosition, double p_DeltaMouseYPosition) {
	double rotationAngleLeftRight = glm::radians(p_DeltaMouseXPosition * m_MouseSensitivity);

	// Rotate around the Y axis - player can rotate left/right, to look around.
	GetComponent<TransformComponent>()->Rotate((float)((m_InverseRotation) ? (rotationAngleLeftRight) : (rotationAngleLeftRight  * -1.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
}