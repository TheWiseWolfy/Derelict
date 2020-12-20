
#include <iostream>
#include <vector>
#include "Entity.h"

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

    Entity& addEntity();

    Entity& rezerveEntity();

};

