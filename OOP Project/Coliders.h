#pragma once

#include "Entity.h"
#include "Components.h"

struct AsteroidColider : Colider{

	AsteroidColider(TranformComponent& _transform, wireframe _vecModel) : Colider(_transform, _vecModel){}

	void onColision(Entity& objectHit)override {




		this->getParentEntity()->destroy();

	}
};

