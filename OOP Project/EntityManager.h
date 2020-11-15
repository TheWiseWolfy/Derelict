
#include <iostream>
#include <vector>
#include "Entity.h"
#include "TextureManager.h"

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

//Temporeri storage to some components

struct CounterComponent : Component
{
    float counter;
    void update(float mFT) override
    {
        counter += mFT;
        std::cout << counter << std::endl;
    }
};


struct KillComponent : Component
{
    CounterComponent& cCounter;

    KillComponent(CounterComponent& mCounterComponent)
        : cCounter(mCounterComponent)
    {
    }

    void update(float mFT) override
    {
        if (cCounter.counter >= 5000) entity->destroy();
    }
};

struct SimpleSprite : Component
{
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;

    SimpleSprite(const char* texturesheet, int x, int y, SDL_Renderer* ren) {
        renderer = ren;
        objTexture = TextureManager::LoadTexture(texturesheet, renderer);


        xpos = x;
        ypos = y;
    }

    void update(float mFT) override
    {
        xpos = 0;
        xpos = 0;

        srcRect.h = 500;
        srcRect.w = 500;
        srcRect.x = 0;
        srcRect.y = 0;

        destRect.h = 500;
        destRect.w = 500;
        destRect.x = 0;
        destRect.y = 0;
    }

    void draw() override
    {
        if (SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect) != 0) {
            std::cout << SDL_GetError() << '\n';
        }
    }

};
