// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

#include "EntityManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "SoundManager.h"

struct AsteroidCollider : Collider{


	AsteroidCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel){}

	void onColision(Entity& objectHit)override {

		Collider::onColision(objectHit);
	}
};

struct PlayerCollider : Collider {

	PlayerCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {
		Collider::onColision(objectHit);

	}
};

struct EnemyCollider : Collider {

	EnemyCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {
		Collider::onColision(objectHit);
	}
};

//Un colider static
struct StationCollider : Collider {

	StationCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {

		Entity* currentEntity = this->getParentEntity();
		Transform& transThis = currentEntity->getComponent<Transform>();
		Collider::onColision(objectHit);
		transThis.setVel(0, 0);

	}
};

//Aici definim logica din spatele proiectilelor din joc
struct ProjectileCollider : Collider {

	ProjectileCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {
		if (objectHit.hasComponent<ProjectileCollider>()) {
			//Collider::onColision(objectHit);

			//this->getParentEntity()->destroy();
		}
		else if (objectHit.hasComponent<AsteroidCollider>()) {
			SoundManager::Instance()->PlaySound("assets/explosion.wav", 0);
			this->getParentEntity()->destroy();
			objectHit.destroy();
		}
		else if (objectHit.hasComponent<EnemyCollider>()) {
			this->getParentEntity()->destroy();
			objectHit.getComponent<EnemyComponent>().onHit();
		}
		else if (objectHit.hasComponent<PlayerCollider>()) {
			this->getParentEntity()->destroy();
			objectHit.getComponent<PlayerComponent>().onHit();
		}
		else if (objectHit.hasComponent<StationCollider>()) {
			this->getParentEntity()->destroy();
		}
	}
};