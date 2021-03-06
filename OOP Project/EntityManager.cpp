// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "EntityManager.h"
#include "Components.h"
#include "Coliders.h"
#include "LevelManager.h"
#include "Game.h"

//Fuctii auxiliare

//!Aici trebuie sa indendificam daca obiectul are oricare tip de colider din joc
bool findCorectCollider(Collider** col, const Entity& en);

//!Aici folosim SAT pentru a detecta coliziuni
bool isIntersecting(const Wireframe& a, const Wireframe& b);

//!Actualizam toate obiectele din joc
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
    }
    reservedEntities.clear();

    //Actualizarea finala
    for (auto& e : entities) {
        e->update(mFT);
    }
   
    collisionCheck();
}

//!Desenam toate obiectele din joc
void EntityManager::draw(){
    for (auto& e : entities) {
        e->draw();
    }
    //This is just debuging
   /* 
    for (size_t f1 = 0; f1 < entities.size(); f1++) {

        Collider* colider1 = nullptr;

        if (findCorectCollider(&colider1, *entities[f1]) ) {

            for (size_t f2 = 0; f2 < colider1->vecModel.size() ; f2++) {
                float x1 = colider1->vecModelinWolrd[f2].first + Level::camera_position.x;
                float y1 = colider1->vecModelinWolrd[f2].second + Level::camera_position.y;
                float x2 = colider1->vecModelinWolrd[(f2 + 1) % colider1->vecModelinWolrd.size()].first + Level::camera_position.x;
                float y2 = colider1->vecModelinWolrd[(f2 + 1) % colider1->vecModelinWolrd.size()].second + Level::camera_position.y ;
                    
                SDL_RenderDrawLine(Game::renderer, (int)x1, (int)y1, (int)x2, (int)y2);
            }
        }
    }
    */

    //Desenam limitele nivelului
    SDL_RenderDrawLine(Game::renderer, 
        1+ Level::camera_position.x,
        1 + Level::camera_position.y,
        Level::levelWidth + Level::camera_position.x,
       1+ Level::camera_position.y);

    SDL_RenderDrawLine(Game::renderer,
        1 + Level::camera_position.x,
        1 + Level::camera_position.y,
        1 + Level::camera_position.x,
        Level::levelHeigh + Level::camera_position.y);

    SDL_RenderDrawLine(Game::renderer,
        1 + Level::camera_position.x,
        Level::levelHeigh + Level::camera_position.y,
        Level::levelWidth + Level::camera_position.x,
        Level::levelHeigh + Level::camera_position.y);

    SDL_RenderDrawLine(Game::renderer,
        Level::levelWidth + Level::camera_position.x,
        1 + Level::camera_position.y,
        Level::levelWidth + Level::camera_position.x,
        Level::levelHeigh + Level::camera_position.y);
}

void EntityManager::clear(){
    entities.clear();
    reservedEntities.clear();
}

//!"Inpachetam" obiectul intr-un smart pointer, si apoi returnam adresa propiuzisa pentru manipulare
Entity& EntityManager::addEntity(){
    Entity* e{ new Entity{} };
    std::unique_ptr<Entity> uPtr( e );
    entities.emplace_back(std::move(uPtr));
    return *e;
}

//!Acelasi lucru ca mai sus, dar acum pentru un vector de asteptare
Entity& EntityManager::rezerveEntity(){

    Entity* e{ new Entity{} };
    std::unique_ptr<Entity> uPtr(e);
    reservedEntities.emplace_back(std::move(uPtr));
    return *e;
}

//!Aici calculez coliziuni prin metoda SAT, pentru fiecare pereche de obiecte cu colider din vector.
void EntityManager::collisionCheck(){

    //Pentru fiecare combinatie de oricare doi colideri
    for (size_t f1 = 0; f1 < entities.size(); f1++) {

        Collider* colider1 = nullptr;
       if (findCorectCollider(&colider1, *entities[f1] ) ) {
            for (size_t f2 = f1 + 1; f2 < entities.size(); f2++) {

                Collider* colider2 = nullptr;
                if (findCorectCollider(&colider2, *entities[f2])  /**/) {

                    auto* entity1 = colider1->getParentEntity();
                    auto* entity2 = colider2->getParentEntity();
                    Vector2D distance = entity1->getComponent<Transform>().position - entity2->getComponent<Transform>().position;

                    //Nu are rost sa verificam coliziunea dintr 2 obiecte care sunt departate unul fata de celalant
                    if (distance.getMagnitude() < 300) {

                        //Separating Axis Theorem Algoritm 
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
}
//!Aici cautam orice componenta care mosteneste de la colider, si apoi face un cast catre tipul colider, ca sa putem chema fuctia onCollision indiferent de ce colider avem pe obiect
bool findCorectCollider(Collider** col,const Entity& en) {
    
    if (en.hasComponent<AsteroidCollider>()  ) {
        *col = &(en.getComponent<AsteroidCollider>());
        return true;
    }
    else if (en.hasComponent<PlayerCollider>() ){
        *col = &(en.getComponent<PlayerCollider>());
        return true;
    }
    else if (en.hasComponent<StationCollider>() ) {
        *col = &(en.getComponent<StationCollider>());
        return true;
    }
    else if (en.hasComponent<ProjectileCollider>() ) {
        *col = &(en.getComponent<ProjectileCollider>());
        return true;
    }
    else if (en.hasComponent<Collider>() ) {
        *col = &(en.getComponent<Collider>());
        return true;
    }
    else if (en.hasComponent<EnemyCollider>() ) {
        *col = &(en.getComponent<EnemyCollider>());
        return true;
    }
    return false;
}

//!Collision detection
bool isIntersecting(const Wireframe& a, const Wireframe& b)
{
    //parcurgem puctele primului poligom
    for (auto i = 0u; i < a.size() + 0; ++i) {
        auto current = a[i];
        auto next = a[(i + 1) % a.size()];

        float xDif = next.first - current.first;
        float yDif = next.second - current.second;

        std::pair<float, float> axis(-yDif, xDif);

        //Calculam proiectile fiecarui puct pe fiecare axa, si determinam puctele minime si maxime
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

       //Daca proiectiile se suprapun atunci continuam, dar daca nu se suprapun, atunci am gasit axa pe care nu exista coliziune, deci nu mai trebuie sa continuam
        if (aMaxProj < bMinProj || aMinProj > bMaxProj) {
            return false;
        }
    }
    //Am verificat toate proiectiile posibile, si obiectele se ating pe toate, deci o coliziune este sigura
    return true;
}
