// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

#include "EntityManager.h"
#include "Vector2D.h"

//O componenta fundamentala care pastreaza coordonatele unui obiect, velocity si calculeaza legatura dintre acestea 2. 
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

//Aici pastram informatie despre coliziunea unui obiect, si o contextualizam in lume. Exista o subclasa de obiecte care mostenesc de la acest obiect
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

//Aici se afla toate controalele si tot ce tine de player
struct PlayerComponent : public Component {

    Transform& transform;
    float playerForce = 5;

    float fireCounter = 0;
    float regenCounter = 0;
    float soundCounter = 0;

    float life = 8;
    float maxLife = 8;

    PlayerComponent(Transform& _transform);

    void update(float mFT) override;


    void draw() override;
    void onHit();

};

//Aici avem AI-ul inamic din joc care urmareste o tinta si trage spre ea
struct EnemyComponent : public Component {

    Transform& transform;
   const Entity& player;

    float counter = 0;
    float life = 3;

    EnemyComponent(const Entity& _player,Transform& _transform);

    void update(float mFT) override;
    void draw() override;

    void onHit();
};

//Aici gestionam sprite-urile dinamica din joc (care se misca dupa un transform)
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

//Aici gestionam sprite-uri simple
struct StaticSprite :public Component {
private:
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    int orizontalSize, verticalSize;
    int spriteRotation;
    int x, y;
    float scrollRate;

public:
    StaticSprite( const char* texturesheet,int x,int y,int h, int w,float scrollRate, int rotation);
    ~StaticSprite();

    void update(float mFT) override;
    void draw() override;
};

//O componenta simpla care trage un proiectil cu un colider special
struct FirearmComponent : public Component {

    Transform& transform;

    FirearmComponent(Transform& _transform);

    void fire();
    void update(float mFT) override;
    void draw() override;
};

//Un timer care autodistruge obiectul
struct SelfDistruct : public Component {

    float counter = 0;

    SelfDistruct();

    void update(float mFT) override;
    void draw() override;
};


