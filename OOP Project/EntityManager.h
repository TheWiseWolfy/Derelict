#pragma once
#include <vector>

#include "Entity.h"

using Wireframe = std::vector<std::pair<float, float>>;

class EntityManager
{
private:
    //parcurgem vectorul in fiecare cadru si chemam fuctiile din interfata: update si draw
    std::vector<std::unique_ptr<Entity>> entities;

    //stocam entitati in asteptare, care vor fi introduse in joc inainte de urmatorul cadru
    std::vector<std::unique_ptr<Entity>> reservedEntities;

public:
    void update(float mFT);
    void draw();

    void clear();
    //Aici putem adauga entitati direct in vectorul parcurs principal, dar asta lucru nu poate fi facut decant intre parcurgerile acestuia
    Entity& addEntity();

    //Aici putem "comanda" entitati, iar jocul le va stoca temporar si va astepta pana le poate muta in vectorul permanent.
    Entity& rezerveEntity();

    //verificam coliziunea dintre obiectele din vector
    void collisionCheck();

};

