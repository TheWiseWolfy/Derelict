#pragma once

#include "Entity.h"
#include "Components.h"
#include "Vector2D.h"

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

struct ProjectileCollider : Collider {

	ProjectileCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {


		if (!objectHit.hasComponent<ProjectileCollider>()) {
			Collider::onColision(objectHit);

			this->getParentEntity()->destroy();
		}
		if (objectHit.hasComponent<AsteroidCollider>()) {
			this->getParentEntity()->destroy();
			objectHit.destroy();
		}
	}
};

struct StationCollider : Collider {

	StationCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {

		Entity* currentEntity = this->getParentEntity();
		Transform& transThis = currentEntity->getComponent<Transform>();
		Collider::onColision(objectHit);
		transThis.setVel(0, 0);

	}
};
