#include <iostream>
#include <math.h>
#include <string>
#include <chrono>
#include <thread>

#include "Game.h"
#include "Components.h"
#include "Coliders.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "TextureManager.h"


//TRANSFORM
Transform::Transform(){

position.x = 0;
position.y = 0;

velocity.x = 0;
velocity.y = 0;

angle = 0;
 }
Transform::Transform(float x, float y, float an) {
    position.x = x;
    position.y = y;

    velocity.x = 0;
    velocity.y = 0;

    angle = an;
}
Transform::Transform(float x, float y, float vx, float vy, float an) {
    position.x = x;
    position.y = y;

    velocity.x = vx;
    velocity.y = vy;

    angle = an;
}
Transform::Transform(const Vector2D& poz, float an) {
    position.x = poz.x;
    position.y = poz.y;

    velocity.x = 0;
    velocity.y = 0;

    angle = an;
}
Transform::Transform(const Vector2D& poz, float an, float _mass)
{
    position.x = poz.x;
    position.y = poz.y;

    velocity.x = 0;
    velocity.y = 0;

    angle = an;
    mass = _mass;
}
Transform::Transform(const Vector2D& poz, const Vector2D& vel, float an) {
    position.x = poz.x;
    position.y = poz.y;

    velocity.x = vel.x;
    velocity.y = vel.y;

    angle = an;
}
Transform::Transform(const Vector2D& poz, const Vector2D& vel, float an, float _mass) {
    position.x = poz.x;
    position.y = poz.y;

    velocity.x = vel.x;
    velocity.y = vel.y;

    angle = an;
    mass = _mass;
}

Transform::Transform(Vector2D* poz, float an) {
     position.x = poz->x;
     position.y = poz->y;

     velocity.x = 0;
     velocity.y = 0;

     angle = an;
 }
Transform::Transform(Vector2D* poz, float an, float _mass)
{
    position.x = poz->x;
    position.y = poz->y;

    velocity.x = 0;
    velocity.y = 0;

    angle = an;
    mass = _mass;
}
Transform::Transform(Vector2D* poz, Vector2D* vel, float an) {
    position.x = poz->x;
    position.y = poz->y;

    velocity.x = vel->x;
    velocity.y = vel->y;
    angle = an;
}
void Transform::setPos(float x, float y) {
    position.x = x;
    position.y = y;
}
void Transform::setPos(Vector2D newPoz)
{
    position = newPoz;
}
void Transform::setVel(float xv, float yv)
{
    velocity.x = xv;
    velocity.y = yv;
}
void Transform::setVel(Vector2D newVec)
{
    velocity = newVec;

}
void Transform::update(float mFT) {

    //Update position based on velocity 
    position.x += velocity.x * mFT / 1000.0f;
    position.y += velocity.y * mFT / 1000.0f;

    if (position.x <= 0)
        velocity.x +=100 ;

    if (position.x >= Level::levelWidth)
        velocity.x -= 100;

    if (position.y <= 0)
        velocity.y += 100;

    if (position.y >= Level::levelHeigh)
        velocity.y -= 100;

    //We create some useful vectors for orientation
    forward.x = cos(angle + M_PI);
    forward.y = sin(angle + M_PI);

    left.x = cos(angle + M_PI / 2);
    left.y = sin(angle + M_PI / 2);

    //We create a soft speed limit
    if (velocity.x < -40.0f) {
        velocity.x += 2.0f;
    }
    else if (velocity.x > 40.0f){
        velocity.x -= 2.0f;
    }

    if (velocity.y < -40.0f){
        velocity.y += 2.0f;
    }
    else if (velocity.y > 40.0f){
        velocity.y -= 2.0f;
    }
}

//COLIDER
//Fuctia asta e o forma simpla de a calcula coliziuni si a aplica niste forte relativ realiste.
Collider::Collider(Transform& _transform, Wireframe _vecModel) : transform(_transform) {

    vecModel = _vecModel;
    vecModelinWolrd = _vecModel;

}
void Collider::onColision(Entity& objectHit){

    Entity* currentEntity = this->getParentEntity();

    Transform& transHit = objectHit.getComponent<Transform>();
    Transform& transThis = currentEntity->getComponent<Transform>();

    Vector2D difference = transThis.position - transHit.position;
    Vector2D relativeVelocity = transHit.velocity + transThis.velocity;

    //std::cout << relativeVelocity.getMagnitude() << " ";
    Vector2D newVelocityThis = std::max(relativeVelocity.getMagnitude(),50.0f) * (1 / transThis.mass) * difference.normalizeVector();
   // Vector2D newVelocityThis = transThis.velocity + difference.normalizeVector() * ( 100 / transThis.mass);
    transThis.setVel(newVelocityThis);
}
void Collider::update(float mFT) {

    // Rotate
    for (int i = 0; i < vecModelinWolrd.size(); i++) {
        vecModelinWolrd[i].first = vecModel[i].first * cos(transform.angle) - vecModel[i].second * sin(transform.angle);
        vecModelinWolrd[i].second = vecModel[i].first * sin(transform.angle) + vecModel[i].second * cos(transform.angle);
    }
    //Translate
    for (int i = 0; i < vecModel.size(); ++i) {
        vecModelinWolrd[i].first = vecModelinWolrd[i].first + transform.position.x;
        vecModelinWolrd[i].second = vecModelinWolrd[i].second + transform.position.y;
    }

}
void Collider::draw()
{

}

//PLAYER COMPONENT
PlayerComponent::PlayerComponent(Transform& _transform) : transform(_transform) {}
void PlayerComponent::update(float mFT) {

    //Set up
    Entity* player = this->getParentEntity();
    int mouseX, mouseY;

    assert(player->hasComponent<FirearmComponent>());


    //Keyboard control
    if (Game::event.type == SDL_KEYDOWN) {
        switch (Game::event.key.keysym.sym) {
        default:
            break;
        }

        //One idea for the controls, might redu later
        if (Game::event.key.keysym.sym == SDLK_UP) {
            transform.velocity += mFT * (playerForce / transform.mass) * transform.forward;

        }
        else if (Game::event.key.keysym.sym == SDLK_DOWN) {
            transform.velocity += mFT * -(playerForce / transform.mass) * transform.forward;
        }

        if (Game::event.key.keysym.sym == SDLK_LEFT) {
            transform.velocity += mFT * (playerForce / transform.mass) * transform.left;
        }
        else if (Game::event.key.keysym.sym == SDLK_RIGHT) {
            transform.velocity += mFT * -(playerForce / transform.mass) * transform.left;
        }
    }
    //Being able too shoot

    //Enemy will shoot at a certain rate
    FirearmComponent* firearm = &(player->getComponent<FirearmComponent>());
    counter += mFT;

    if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_SPACE) {
        if (counter >= 1000) {
            firearm->fire();
            counter = 0;
        }
   }

    //Control mouse
    if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT)) {

        Vector2D gameMouse = Level::screenSpaceToGameSpace(mouseX, mouseY);
        float derisedAngle = atan2(transform.position.y - gameMouse.y, transform.position.x - gameMouse.x);
       transform.angle = derisedAngle;
      
       /*
       std::cout <<"  ----------------------   " << std::endl;
       std::cout << mouseY << "  " << mouseX << std::endl;
       std::cout << "Ship position: X: " << transform.position.x << "Y: " << transform.position.y << " \n";
       std::cout <<"Mouse game position: X: "<< gameMouse.x <<"Y: "<< gameMouse.y << endl;
       std::cout << transform.position.x - gameMouse.x << "  " << transform.position.y - gameMouse.y << endl;
       std::cout << "Curent angle is:" << transform.angle << "\n";  
       std::cout << "Curent desired angle is:" << derisedAngle << "\n"; 
        */
        
    }
    
    //We update the camera
    Level::camera_position.x = -transform.position.x + Level::camera_size.x / 2;
    Level::camera_position.y = -transform.position.y + Level::camera_size.y / 2;

    //If the player has no life, we destryo the player
    //std::cout << life << "\n";
    if (life <= 0) {
       // player->destroy();
    }

}
void PlayerComponent::draw()
{
    //optional debug line
    SDL_RenderDrawLine(Game::renderer,
        transform.position.x + Level::camera_position.x,
        transform.position.y + Level::camera_position.y,
        transform.position.x + 100 * transform.forward.x + Level::camera_position.x,
        transform.position.y + 100 * transform.forward.y + Level::camera_position.y
    );
}
void PlayerComponent::onHit() {
    life -= 1;
}

//ENEMY AI COMPONENT

EnemyComponent::EnemyComponent(Entity& _player, Transform& _transform) : player(_player), transform(_transform)
{
}
void EnemyComponent::update(float mFT){

    //Set up
    Entity* enemy = this->getParentEntity();
    auto& playerTransform = player.getComponent<Transform>();


    //If we have no firearm then we can not proceed
    assert(enemy->hasComponent<FirearmComponent>() );

    //Enemy will always face the player
    Vector2D playerPoz = playerTransform.position;
    float derisedAngle = atan2(transform.position.y - playerPoz.y, transform.position.x - playerPoz.x);
    transform.angle = derisedAngle;

    //Enemy will keep a proper distance from the player

    Vector2D player_enemy = transform.position - playerTransform.position;

    float distance = player_enemy.getMagnitude();

    if (distance > 300.0f) {
        transform.velocity += mFT * (1 / transform.mass) * transform.forward;

    }
    else if (distance < 800.0f) {
        transform.velocity += mFT * -(1 / transform.mass) * transform.forward;

    }

    //Enemy will shoot at a certain rate
    FirearmComponent* firearm = &(enemy->getComponent<FirearmComponent>() );
    counter += mFT;
    if (counter >= 1000 && distance < 600.0f) {
        firearm->fire();
        counter = 0;
    }

    if (life <= 0) {
      enemy->destroy();
    }
}
void EnemyComponent::draw(){

}
void EnemyComponent::onHit(){
    life -= 1;
}

//SIMPLE SPRITE
SimpleSprite::SimpleSprite(Transform& _transform, const char* texturesheet, int h, int w, int rotation) :transform(_transform) {

    objTexture = TextureManager::LoadTexture(texturesheet, Game::renderer);

    //We store the size of the texture
    orizontalSize = TextureManager::getsize(objTexture).x;
    verticalSize = TextureManager::getsize(objTexture).y;

    //We define the map as the entire image.
    srcRect.h = verticalSize;
    srcRect.w =  orizontalSize;
    srcRect.x = 0;
    srcRect.y = 0;

    //We place it in the correct spot
    destRect.h = h;
    destRect.w = w;

    spriteRotation = rotation;
}
SimpleSprite::~SimpleSprite(){
    SDL_DestroyTexture(objTexture);
}
void SimpleSprite::update(float mFT) {
    //Draw the sprite at the position we desire

    destRect.x = Level::camera_position.x + transform.x() - destRect.h / 2.0f;
    destRect.y = Level::camera_position.y + transform.y() - destRect.w / 2.0f;
}
void SimpleSprite::draw() {
    //render in window space
    if (SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect, ( /**/(transform.angle * 180) / M_PI + spriteRotation),
        NULL, SDL_FLIP_NONE) != 0)
    {
        std::cout << SDL_GetError() << '\n';
    }
}

//STATIC SPRITE
StaticSprite::StaticSprite( const char* texturesheet, int _x, int _y,int h, int w,float _scroll,int rotation) {
    objTexture = TextureManager::LoadTexture(texturesheet, Game::renderer);

    //We store the size of the texture
    orizontalSize = TextureManager::getsize(objTexture).x;
    verticalSize = TextureManager::getsize(objTexture).y;

    //We define the map as the entire image.
    srcRect.h = verticalSize;
    srcRect.w = orizontalSize;
    srcRect.x = 0;
    srcRect.y = 0;

    //We place it in the correct spot
    destRect.h = h;
    destRect.w = w;
    destRect.x = 0;
    destRect.y = 0;

    x = _x;
    y= _y;
    spriteRotation = rotation;
    scroll = _scroll;
}
StaticSprite::~StaticSprite(){
    SDL_DestroyTexture(objTexture);
}
void StaticSprite::update(float mFT){
 
    if (scroll != 0){
    destRect.x = Level::camera_position.x / scroll + x ;
    destRect.y = Level::camera_position.y / scroll + y;
    }
}
void StaticSprite::draw(){

  
    if (SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect,spriteRotation,
        NULL, SDL_FLIP_NONE) != 0)
    {
        std::cout << SDL_GetError() << '\n';
    }

   
}


//FIREARM COMPONENT
FirearmComponent::FirearmComponent(Transform& _transform) : transform(_transform) {}
void FirearmComponent::fire() {
   auto& rocket(Game::entityManager.rezerveEntity());

   Wireframe vecModelShip = {
   {-25.0,0},
   {25.0f,-25.0f},
   {25.0f,25.0f}
   };

   Vector2D poz(transform.position.x, transform.position.y);
    auto& rocket_transform(rocket.addComponent<Transform>(poz + transform.forward * 70, 1000 * transform.forward + transform.velocity, transform.angle ,3)/**/);
    auto& rochet_colider(rocket.addComponent<ProjectileCollider>(rocket_transform, vecModelShip));
    auto& rocket_transform_sprite(rocket.addComponent<SimpleSprite>(rocket_transform, "assets/bullet.png", 40, 40, 180) /**/);
    auto& SelfDistruct(rocket.addComponent<SelfDistruct>());
}
void FirearmComponent::update(float mFT) {

}
void FirearmComponent::draw()
{

}

//SELF DISTRUCT COMPONENT
SelfDistruct::SelfDistruct(){
}
void SelfDistruct::update(float mFT){

    //creating a teoretical max FPS of 1000 for stability reasons, not very portable
    counter += mFT;
    //std::cout << counter << std::endl;
    if (counter >= 1000) this->getParentEntity()->destroy();

}
void SelfDistruct::draw()
{
}
