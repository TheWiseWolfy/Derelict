#pragma once

#include "Game.h"
#include "Vector2D.h"

//O componenta fundamentala care pastreaza coordonatele unui obiect, velocity si calculeaza legatura dintre acestea 2. 
//Aditional am creat niste vectori locali folositori cum ar fi forward.
struct TranformComponent : public Component{
    //Position is game space
    Vector2D position;
    float angle;

public:
    Vector2D velocity;

    Vector2D forward;  //A vector that always points forward 
    Vector2D left; 

    TranformComponent();
    TranformComponent(float x, float y, float an);
    TranformComponent(float x, float y, float vx, float vy, float an);
    TranformComponent(const Vector2D& poz, float an);
    TranformComponent(const Vector2D& poz, const Vector2D& vel, float an);

    TranformComponent(Vector2D* poz, float an);
    TranformComponent(Vector2D* poz, Vector2D* vel, float an);

    inline float x() { return position.x; }
    inline float y() { return  position.y; }

    void setPos(float x, float y);

    void update(float mFT) override;
};

struct Colider : public Component {
protected:
    TranformComponent& transform;
public:

    virtual void onColision(Entity& objectHit);

    std::vector<std::pair<float, float>> vecModel;
    std::vector<std::pair<float, float>> vecModelinWolrd;

    Colider(TranformComponent& _transform, wireframe _vecModel);

    void update(float mFT) override;
    void draw() override;
};

struct PlayerComponent : public Component {

    TranformComponent& transform;
    float plAc = 1.3;

    PlayerComponent(TranformComponent& _transform);

    void update(float mFT) override;


    void draw() override;
};

struct SimpleSprite :public Component {
private:
    TranformComponent& transform;
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    int orizontalSize, verticalSize;
    int spriteRotation;

public:
    SimpleSprite(TranformComponent& _transform, const char* texturesheet, int h, int w, int rotation);

    void update(float mFT) override;

    void draw() override;
};

struct FirearmComponent : public Component {

    TranformComponent& transform;

    FirearmComponent(TranformComponent& _transform);

    void fire();

    void update(float mFT) override;

    void draw() override;
};

