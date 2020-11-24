#include "Entity.h"


void Entity::update(float mFT)
{
    for (auto& c : components) c->update(mFT);
}

void Entity::draw()
{
    for (auto& c : components) c->draw();
}


