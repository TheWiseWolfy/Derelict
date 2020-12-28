#include "EntityManager.h"
#include "Components.h"

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
       


    //Update every component of 
    for (auto& e : entities) {
        std::cout <<"inside "<< e << "\n";
        e->update(mFT);
       // std::cout << "Colider" << e->hasComponent<Colider>() << "\n";
    }

    //Aici calculam coliziunea dintre obiecte.
    collisionCheck();

    std::cout << '\n';

}

void EntityManager::draw(){
    for (auto& e : entities) e->draw();

    //This is just debuging

    for (int f1 = 0; f1 < entities.size(); f1++) {
        if (entities[f1]->hasComponent<Colider>()  /**/) {

                    auto tranform1 = entities[f1]->getComponent<TranformComponent>();

                    auto colider1 = entities[f1]->getComponent<Colider>();

                    for (int f2 = 0; f2 < colider1.vecModel.size() -1; f2++) {
                        float x1 = tranform1.position.x + colider1.vecModel[f2].first + Level::camera_position.x;
                        float y1 = tranform1.position.y + colider1.vecModel[f2].second + Level::camera_position.y;
                        float x2 = tranform1.position.x + colider1.vecModel[f2+ 1].first + Level::camera_position.x;
                        float y2 = tranform1.position.y + colider1.vecModel[f2 + 1].second + Level::camera_position.y ;
                    
                        SDL_RenderDrawLine(Game::renderer, x1, y1, x2, y2);
                    }
                }
            }
        
    


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

//Aici calculez coliziuni prin metoda SAT, pentru fiecare pereche de obiecte cu colider din vector.
void EntityManager::collisionCheck(){

    //Update every component of 
    for (int f1 = 0; f1 < entities.size(); f1++) {
        if (entities[f1]->hasComponent<Colider>()  /**/ ) {
            //Pentru fiecare entitate cu un colider, verifica toate relatile ramase
            for (int f2 = f1 + 1; f2 < entities.size(); f2++) {
                if (entities[f2]->hasComponent<Colider>()  /**/) {

                    auto tranform1 = entities[f1]->getComponent<TranformComponent>();
                    auto tranform2 = entities[f2]->getComponent<TranformComponent>();

                    auto colider1 = entities[f1]->getComponent<Colider>();
                    auto colider2 = entities[f2]->getComponent<Colider>();

                    //Separating Axis Theorem Algoritm implementat de mine

                    float x1 = tranform1.position.x + colider1.vecModel[f1].first;
                    float y1 = tranform1.position.y + colider1.vecModel[f1].second;

                    float x2 = tranform1.position.x + colider1.vecModel[f1 + 1].first;
                    float y2 = tranform1.position.y + colider1.vecModel[f1 + 1].second;


                    //calculam panta axei pe care o studiem 

                    float m = (y2 - y1) / (x1 - x2);

                    //

                }
            }
        }
    }


}
