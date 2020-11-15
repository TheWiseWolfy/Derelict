#include <iostream>
#include <vector>
#include<SDL.h>

// Forward-declaration of the `Entity` class.
class Entity;

struct Component
{
public:
    int xpos;
    int ypos;
    SDL_Renderer* renderer;

public:
    // We will use a pointer to store the parent entity. A typical game component will encapsulate data, logic and drawing behaviour
    Entity* entity{ nullptr };

    //two metods that will be overridden by game component types
    virtual void update(float mFT) {}
    virtual void draw() {}
 
    //this class also requires a virtual destructor.
    virtual ~Component() {}


};

// Even if the `Entity` class may seem complex, conceptually it is
// very simple. Just think of an entity as a container for components,
// with syntatic sugar methods to quicky add/update/draw components.


class Entity
{
private:
    bool alive = true;  //the entity start out alive

    std::vector<std::unique_ptr<Component>> components;      //Eatch entity will contain a number of components
public:

    // Updating and drawing simply consists in updating and drawing all the components.
    void update(float mFT);
    void draw();

    // We will also define some methods to control the lifetime
    // of the entity.
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c = new T(std::forward<TArgs>(mArgs)...);   //cream o componenta noua de tim component cu un numar arbitrar de argumente

        c->entity = this;

        std::unique_ptr<Component> uPtr(c);


        components.emplace_back(std::move(uPtr));


        return *c;

    }
};


