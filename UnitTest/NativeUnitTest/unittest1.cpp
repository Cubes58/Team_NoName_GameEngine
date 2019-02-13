#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTest {		
	TEST_CLASS(ColisionTests) {
	public:
		TEST_METHOD(TrueCase) {
			Collision collision;

			glm::vec3 entityOnePosition(0.0f, 0.0f, 0.0f);
			glm::vec3 entityOneSize(5.0f, 5.0f, 5.0f);

			glm::vec3 entityTwoPosition(0.0f, 0.0f, 0.0f);
			glm::vec3 entityTwoSize(5.0f, 5.0f, 5.0f);

			Assert::IsTrue(collision(entityOnePosition, entityTwoPosition, entityOneSize, entityTwoSize));
		}

		TEST_METHOD(FalseCase) {
			Collision collision;

			glm::vec3 entityOnePosition(100.0f, 0.0f, 100.0f);
			glm::vec3 entityOneSize(5.0f, 5.0f, 5.0f);

			glm::vec3 entityTwoPosition(0.0f, 0.0f, 0.0f);
			glm::vec3 entityTwoSize(5.0f, 5.0f, 5.0f);

			Assert::IsFalse(collision(entityOnePosition, entityTwoPosition, entityOneSize, entityTwoSize));
		}
	};

	TEST_CLASS(CameraTests) {
	public:
		TEST_METHOD(CreateCamera) {
			glm::vec3 position(0.0f, 0.0f, 0.0f);
			glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);

			CameraComponent cameraComponent(position, orientation);

			bool result = false;
			if (cameraComponent.m_Position == position && cameraComponent.m_Orientation == orientation)
				result = true;

			Assert::IsTrue(result);
		}
	};

	TEST_CLASS(AttackComponentTests) {
	public:
		TEST_METHOD(AttackTimeWaitTest) {
			float attackDamage = 10.0f;
			float attackDistance = 40.0f;
			float attackSpeed = 25.0f;

			AttackComponent attackComponent(attackDamage, attackDistance, attackSpeed);
			attackComponent.OnMessage("Attack");

			Assert::IsFalse(attackComponent.CanAttack());

		}

		TEST_METHOD(AttackTimeResetWaitTest) {
			float attackDamage = 10.0f;
			float attackDistance = 40.0f;
			float attackSpeed = 25.0f;

			AttackComponent attackComponent(attackDamage, attackDistance, attackSpeed);
			attackComponent.OnMessage("Attack");
			attackComponent.OnUpdate(attackSpeed);

			Assert::IsTrue(attackComponent.CanAttack());

		}
	};

	TEST_CLASS(TransformComponentTests) {
	public:
		TEST_METHOD(TranslateTest) {
			glm::vec3 position(0.0f, 0.0f, 0.0f);
			glm::vec3 translateVector(10.0f, 0.0f, 0.0f);

			TransformComponent transformComponent(position);
			transformComponent.Translate(translateVector);

			bool result = false;
			if (position.x + translateVector.x == transformComponent.m_Position.x 
				&& position.y + translateVector.y == transformComponent.m_Position.y 
				&& position.z + translateVector.z == transformComponent.m_Position.z)
				result = true;

			Assert::IsTrue(result);
		}

		TEST_METHOD(ScaleTest) {
			glm::vec3 position(0.0f, 0.0f, 0.0f);
			glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);

			glm::vec3 scale(1.0f, 1.0f, 1.0f);
			glm::vec3 scaleVector(0.1f, 0.1f, 0.1f);

			TransformComponent transformComponent(position, orientation, scale);
			transformComponent.ScaleUp(scaleVector);

			bool result = false;
			if (scale.x + scaleVector.x == transformComponent.m_Scale.x
				&& scale.y + scaleVector.y == transformComponent.m_Scale.y
				&& scale.z + scaleVector.z == transformComponent.m_Scale.z)
				result = true;

			Assert::IsTrue(result);
		}
	};

	TEST_CLASS(HealthComponentTests) {
	public:
		TEST_METHOD(TakeDamageTest) {
			float health = 100.0f;

			HealthComponent healthComponent(health);
			healthComponent.OnMessage("HitByEnemyTower");

			Assert::AreNotEqual(health, healthComponent.GetHealth());
		}

		TEST_METHOD(HealthBelowZeroTest) {
			float health = 100.0f;

			HealthComponent healthComponent(health);

			for (int i = 0; healthComponent.GetHealth() > 0; i++) {
				healthComponent.OnMessage("HitByEnemyTower");

				if (i >= 1000)
					break;
			}
			healthComponent.OnUpdate(1.0f);
			Assert::IsTrue(healthComponent.IsHealthBelowZero());
		}

		TEST_METHOD(ResetTest) {
			float health = 100.0f;

			HealthComponent healthComponent(health);
			healthComponent.OnMessage("HitByEnemyTower");

			healthComponent.OnMessage("Reset");
			Assert::AreEqual(healthComponent.GetHealth(), health);
		}
	};
	
	TEST_CLASS(MovementComponentTests) {
	public:
		TEST_METHOD(ChangeAcceleration) {
			glm::vec3 velocity(0.0f, 0.0f, 0.0f);
			glm::vec3 acceleration(1.0f, 0.0f, 0.0f);

			MovementComponent movementComponent(velocity, acceleration);
			movementComponent.OnMessage("ChangeAcceleration");

			bool same = false;
			if (acceleration.x == movementComponent.m_Acceleration.x
				&& acceleration.y == movementComponent.m_Acceleration.y
				&& acceleration.z == movementComponent.m_Acceleration.z)
				same = true;

			Assert::IsFalse(same);
		}

		TEST_METHOD(UpdateVelocity) {
			glm::vec3 velocity(0.0f, 0.0f, 0.0f);
			glm::vec3 acceleration(1.0f, 0.0f, 0.0f);

			MovementComponent movementComponent(velocity, acceleration);
			movementComponent.OnUpdate(500.0f);

			bool same = false;
			if (velocity.x == movementComponent.m_Velocity.x
				&& velocity.y == movementComponent.m_Velocity.y
				&& velocity.z == movementComponent.m_Velocity.z)
				same = true;

			Assert::IsFalse(same);
		}

		TEST_METHOD(KeepsTrackOfDeltaTime) {
			glm::vec3 velocity(0.0f, 0.0f, 0.0f);
			glm::vec3 acceleration(1.0f, 0.0f, 0.0f);


			MovementComponent movementComponent(velocity, acceleration);
			float timePassed = 425.0f;
			movementComponent.OnUpdate(timePassed);

			Assert::AreEqual(movementComponent.m_TimePassed, timePassed);
		}
	};
}