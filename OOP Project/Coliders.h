#pragma once

#include "Entity.h"
#include "Components.h"

struct AsteroidCollider : Collider{

	AsteroidCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel){}

	void onColision(Entity& objectHit)override {
		this->getParentEntity()->destroy();
	}
};

struct PlayerCollider : Collider {

	PlayerCollider(Transform& _transform, Wireframe _vecModel) : Collider(_transform, _vecModel) {}

	void onColision(Entity& objectHit)override {
	
	}
};

