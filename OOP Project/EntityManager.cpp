#include "EntityManager.h"


void  EntityManager::update(float mFT)
{
    //Tot ce se intampla aici se intampla intre frame-uri
    //Cautam entitati moarte
    
    entities.erase(
        std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isAlive();
            }),
        std::end(entities));

    //Adaugam entitati importante in vectorul principal
    int size = reservedEntities.size();

    for (int i = 0; i < size; ++i) {
        entities.emplace_back(std::move(reservedEntities.at(i)));
        reservedEntities.erase(reservedEntities.begin() + i);
    }
       
    for (auto& e : entities) {
        std::cout <<"inside "<< e << "\n";
        e->update(mFT);
    }

    std::cout << '\n';

}

void EntityManager::draw(){
    for (auto& e : entities) e->draw();
}

Entity& EntityManager::addEntity(){

    Entity* e{ new Entity{} };
    std::unique_ptr<Entity> uPtr( e );
    entities.emplace_back(std::move(uPtr));
    return *e;
}

Entity& EntityManager::rezerveEntity(){

    Entity* e{ new Entity{} };
    std::unique_ptr<Entity> uPtr(e);
    reservedEntities.emplace_back(std::move(uPtr));
    return *e;
}
