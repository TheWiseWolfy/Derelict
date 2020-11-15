#include "EntityManager.h"


void  EntityManager::update(float mFT)
{
    // We will start by cleaning up "dead" entities.
    entities.erase(
        std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isAlive();
            }),
        std::end(entities));

    // This algorithm closely resembles the one we used in
    // the first episode of the series to delete "destroyed"
    // blocks. Basically, we're going through all entities and
    // erasing the "dead" ones.
    // This is known as the "erase-remove idiom".

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