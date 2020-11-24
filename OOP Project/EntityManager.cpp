#include "EntityManager.h"


void  EntityManager::update(float mFT)
{
    //Cautam entitati moarte
    entities.erase(
        std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isAlive();
            }),
        std::end(entities));

    for (auto& e : entities) e->update(mFT);
}

void EntityManager::draw()
{
    for (auto& e : entities) e->draw();
}

Entity& EntityManager::addEntity()
{
    Entity* e{ new Entity{} };

    std::unique_ptr<Entity> uPtr( e );

    entities.emplace_back(std::move(uPtr));

    return *e;
}