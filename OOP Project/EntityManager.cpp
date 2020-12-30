// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "EntityManager.h"
#include "Components.h"
#include "Coliders.h"

//Auxiliary fuctions 
bool findCorectCollider(Collider** col, const Entity& en);
bool isIntersecting(const Wireframe& a, const Wireframe& b);


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
        std::cout << e<<" ";
        e->update(mFT);
    }
    std::cout << "\n";

    collisionCheck();
}

void EntityManager::draw(){
    for (auto& e : entities) {
        e->draw();
    }
    //This is just debuging

    for (size_t f1 = 0; f1 < entities.size(); f1++) {

        Collider* colider1 = nullptr;

        if (findCorectCollider(&colider1, *entities[f1]) /**/) {

            for (size_t f2 = 0; f2 < colider1->vecModel.size() ; f2++) {
                float x1 = colider1->vecModelinWolrd[f2].first + Level::camera_position.x;
                float y1 = colider1->vecModelinWolrd[f2].second + Level::camera_position.y;
                float x2 = colider1->vecModelinWolrd[(f2 + 1) % colider1->vecModelinWolrd.size()].first + Level::camera_position.x;
                float y2 = colider1->vecModelinWolrd[(f2 + 1) % colider1->vecModelinWolrd.size()].second + Level::camera_position.y ;
                    
                SDL_RenderDrawLine(Game::renderer, (int)x1, (int)y1, (int)x2, (int)y2);
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
    for (size_t f1 = 0; f1 < entities.size(); f1++) {

        Collider* colider1 = nullptr;

       if (findCorectCollider(&colider1, *entities[f1] ) ) {
            //Pentru fiecare entitate cu un colider, verifica toate relatile ramase
            for (size_t f2 = f1 + 1; f2 < entities.size(); f2++) {

                Collider* colider2 = nullptr;

                if (findCorectCollider(&colider2, *entities[f2])  /**/) {

                    //Separating Axis Theorem Algoritm implementat de mine

                    if (isIntersecting(colider1->vecModelinWolrd, colider2->vecModelinWolrd)) {
                       // std::cout << "Colider";

                        colider1->onColision(*(colider2->getParentEntity()));
                        colider2->onColision(*(colider1->getParentEntity()));
                    }
                }
            }
        }
    }
}
    
bool findCorectCollider(Collider** col,const Entity& en) {
    
    if (en.hasComponent<AsteroidCollider>()  /**/) {
        *col = &(en.getComponent<AsteroidCollider>());
        return true;
    }
    else if (en.hasComponent<PlayerCollider>() /**/){
        *col = &(en.getComponent<PlayerCollider>());
        return true;
    }
    return false;
}

bool isIntersecting(const Wireframe& a, const Wireframe& b)
{
    // loop over the vertices(-> edges -> axis) of the first polygon
    for (auto i = 0u; i < a.size() + 0; ++i) {
        // calculate the normal vector of the current edge
        // this is the axis will we check in this loop
        auto current = a[i];
        auto next = a[(i + 1) % a.size()];

        float xDif = next.first - current.first;
        float yDif = next.second - current.second;

        std::pair<float, float> axis(-yDif, xDif);

        // loop over all vertices of both polygons and project them
        // onto the axis. We are only interested in max/min projections
        auto aMaxProj = -std::numeric_limits<float>::infinity();
        auto aMinProj = std::numeric_limits<float>::infinity();
        auto bMaxProj = -std::numeric_limits<float>::infinity();
        auto bMinProj = std::numeric_limits<float>::infinity();

        for (const auto& v : a) {
            auto proj = v.first * axis.first + v.second * axis.second;
            if (proj < aMinProj) aMinProj = proj;
            if (proj > aMaxProj) aMaxProj = proj;
        }

        for (const auto& v : b) {
            auto proj = v.first * axis.first + v.second * axis.second;
            if (proj < bMinProj) bMinProj = proj;
            if (proj > bMaxProj) bMaxProj = proj;
        }

        // now check if the intervals the both polygons projected on the
        // axis overlap. If they don't, we have found an axis of separation and
        // the given polygons cannot overlap
        if (aMaxProj < bMinProj || aMinProj > bMaxProj) {
            return false;
        }
    }

    // at this point, we have checked all axis but found no separating axis
    // which means that the polygons must intersect.
    return true;
}
