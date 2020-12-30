#pragma once
#include <vector>

#include "Entity.h"

using Wireframe = std::vector<std::pair<float, float>>;

// If `Entity` is an aggregate of components, `Manager` is an aggregate
// of entities. Implementation is straightforward, and resembles the
// previous one.

class EntityManager
{
private:
    //this is the vector that is being used every frame to update every active entity
    std::vector<std::unique_ptr<Entity>> entities;

    //this vector is used for standby entities that have to been introduced in the vector in between frames
    std::vector<std::unique_ptr<Entity>> reservedEntities;

public:
    void update(float mFT);
    void draw();

    //Aici putem adauga entitati direct in vectorul parcurs regulat, dar asta nu poate fi facut decant intre parcurgerile regulate ale 
    //vectorului
    Entity& addEntity();

    //Aici putem "comanda" entitati, iar jocul le va stoca temporar si va astepta pana le poate muta in vectorul permanent.
    Entity& rezerveEntity();

    //aici se intampla magia
    void collisionCheck();

};

