
#include <iostream>
#include <vector>
#include "Entity.h"

// If `Entity` is an aggregate of components, `Manager` is an aggregate
// of entities. Implementation is straightforward, and resembles the
// previous one.

class EntityManager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
public:
    void update(float mFT);
    void draw();

    Entity& addEntity();
};

