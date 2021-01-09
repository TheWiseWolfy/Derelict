// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Entity.h"

void Entity::update(float mFT)
{
    for (auto& c : components) c->update(mFT);
}

void Entity::draw()
{
    for (auto& c : components) c->draw();
}


