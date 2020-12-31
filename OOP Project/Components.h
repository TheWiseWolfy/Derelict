#pragma once

#include <chrono>
#include <thread>

#include "EntityManager.h"
#include "Vector2D.h"

//O componenta fundamentala care pastreaza coordonatele unui obiect, velocity si calculeaza legatura dintre acestea 2. 
//Aditional am creat niste vectori locali folositori cum ar fi forward.
struct Transform : public Component{
    //Position is game space
    Vector2D position;
    float angle;

public:
    Vector2D velocity;
    float mass = 1;

    Vector2D forward;  //A vector that always points forward 
    Vector2D left; 

    Transform();
    Transform(float x, float y, float an);
    Transform(float x, float y, float vx, float vy, float an);
    Transform(const Vector2D& poz, float an);
    Transform(const Vector2D& poz, float an,float mass);
    Transform(const Vector2D& poz, const Vector2D& vel, float an, float _mass);
       

    Transform(const Vector2D& poz, const Vector2D& vel, float an);

    Transform(Vector2D* poz, float an);
    Transform(Vector2D* poz, float an, float _mass);

    Transform(Vector2D* poz, Vector2D* vel, float an);

    inline float x() { return position.x; }
    inline float y() { return  position.y; }

    void setPos(float x, float y);
    void setPos(Vector2D newPoz);

    void setVel(float xv, float yv);
    void setVel(Vector2D newVec);

    void update(float mFT) override;
};

struct Collider : public Component {
protected:
    Transform& transform;
public:

    virtual void onColision(Entity& objectHit);

    std::vector<std::pair<float, float>> vecModel;
    std::vector<std::pair<float, float>> vecModelinWolrd;   

    Collider(Transform& _transform, Wireframe _vecModel);

    void update(float mFT) override;
    void draw() override;
};

struct PlayerComponent : public Component {

    Transform& transform;
    float playerForce = 5;

    PlayerComponent(Transform& _transform);

    void update(float mFT) override;


    void draw() override;
};

struct SimpleSprite :public Component {
private:
    Transform& transform;
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    int orizontalSize, verticalSize;
    int spriteRotation;

public:
    SimpleSprite(Transform& _transform, const char* texturesheet, int h, int w, int rotation);
    ~SimpleSprite();
    void update(float mFT) override;

    void draw() override;
};

struct StaticSprite :public Component {
private:
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    int orizontalSize, verticalSize;
    int spriteRotation,scroll;
    int x, y;

public:
    StaticSprite( const char* texturesheet,int x,int y,int h, int w,float scroll, int rotation);
    ~StaticSprite();
    void update(float mFT) override;

    void draw() override;
};

struct FirearmComponent : public Component {

    Transform& transform;

    FirearmComponent(Transform& _transform);

    void fire();

    void update(float mFT) override;

    void draw() override;
};

struct SelfDistruct : public Component {

    float counter = 0;

    SelfDistruct();


    void update(float mFT) override;

    void draw() override;
};


