#include <iostream>
#include <math.h>
#include <string>
#include <chrono>
#include <thread>

#include "Components.h"
#include "Coliders.h"
#include "EntityManager.h"
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
Transform::Transform(const Vector2D& poz, const Vector2D& vel, float an) {
    position.x = poz.x;
    position.y = poz.y;

    velocity.x = vel.x;
    velocity.y = vel.y;

    angle = an;
}
Transform::Transform(Vector2D* poz, float an) {
     position.x = poz->x;
     position.y = poz->y;

     velocity.x = 0;
     velocity.y = 0;

     angle = an;
 }
Transform::Transform(Vector2D* poz, Vector2D* vel, float an) {
    position.x = poz->x;
    position.y = poz->y;

    velocity.x = vel->x;
    velocity.y = vel->y;
    angle = an;
}
inline void Transform::setPos(float x, float y) {
    position.x = x;
    position.y = y;
}
void Transform::update(float mFT) {

    //Update position based on velocity 
    position.x += velocity.x * mFT / 1000.0f;
    position.y += velocity.y * mFT / 1000.0f;

    //cout << velocity.x <<" "<<velocity.y << " \n";
    //Look, so this is broken, think about how to do it properly
    if (position.x <= 0)
        position.x = 0;

    if (position.x >= Level::levelWidth)
        position.x = Level::levelWidth;

    if (position.y <= 0)
        position.y = 0;

    if (position.y >= Level::levelHeigh)
        position.y = Level::levelHeigh;

    //We create some useful vectors for orientation
    forward.x = cos(angle + M_PI);
    forward.y = sin(angle + M_PI);

    left.x = cos(angle + M_PI / 2);
    left.y = sin(angle + M_PI / 2);
}
inline void Collider::onColision(Entity& objectHit) {
    std::cout << "This should not run\n";
}

//COLIDER
Collider::Collider(Transform& _transform, Wireframe _vecModel) : transform(_transform) {

    vecModel = _vecModel;
    vecModelinWolrd = _vecModel;

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
    int mouseX, mouseY;

    //cout << transform.forward * 100 << "\n";

    if (Game::event.type == SDL_KEYDOWN) {
        switch (Game::event.key.keysym.sym) {
        default:
            break;
        }

        //One idea for the controls, might redu later
        if (Game::event.key.keysym.sym == SDLK_UP) {
            transform.velocity += mFT * plAc * transform.forward;
        }
        else if (Game::event.key.keysym.sym == SDLK_DOWN) {
            transform.velocity += mFT * -plAc * transform.forward;
        }

        if (Game::event.key.keysym.sym == SDLK_LEFT) {
            transform.velocity += mFT * plAc * transform.left;
        }
        else if (Game::event.key.keysym.sym == SDLK_RIGHT) {
            transform.velocity += mFT * -plAc * transform.left;
        }
    }

    //To redo
    if (transform.velocity.x > 0)
        transform.velocity.x -= 0.5f;
    else
        transform.velocity.x += 0.5f;

    if (transform.velocity.y > 0)
        transform.velocity.y -= 0.5f;
    else
        transform.velocity.y += 0.5f;

    //Control mouse
    if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT)) {

        Vector2D gameMouse = Level::screenSpaceToGameSpace(mouseX, mouseY);
        transform.angle = atan2(transform.position.y - gameMouse.y, transform.position.x - gameMouse.x);

        /*
        std::cout <<"  ----------------------   " << endl;
        std::cout << mouseY << "  " << mouseX << endl;
        std::cout << "Ship position: X: " << transform.position.x << "Y: " << transform.position.y << " \n";
        std::cout <<"Mouse game position: X: "<< gameMouse.x <<"Y: "<< gameMouse.y << endl;
        std::cout << transform.position.x - gameMouse.x << "  " << transform.position.y - gameMouse.y << endl;
        std::cout << "Curent angle is:" << transform.angle << "\n"; //* 180 / M_PI << endl;
        */
    }
    Level::camera_position.x = -transform.position.x + Level::camera_size.x / 2;
    Level::camera_position.y = -transform.position.y + Level::camera_size.y / 2;
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

//SIMPLE SPRITE
SimpleSprite::SimpleSprite(Transform& _transform, const char* texturesheet, int h, int w, int rotation) :transform(_transform) {

    objTexture = TextureManager::LoadTexture(texturesheet, Game::renderer);

    //We store the size of the texture
    orizontalSize = TextureManager::getsize(objTexture).x;
    verticalSize = TextureManager::getsize(objTexture).y;

    //We define the map as the entire image.
    srcRect.h = orizontalSize;
    srcRect.w = verticalSize;
    srcRect.x = 0;
    srcRect.y = 0;

    //We place it in the correct spot
    destRect.h = h;
    destRect.w = w;

    spriteRotation = rotation;
}
SimpleSprite::~SimpleSprite()
{
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
    auto& rocket_transform(rocket.addComponent<Transform>(poz, 1000 * transform.forward + transform.velocity, transform.angle )/**/);
    auto& rochet_colider(rocket.addComponent<PlayerCollider>(rocket_transform, vecModelShip));
    auto& rocket_transform_sprite(rocket.addComponent<SimpleSprite>(rocket_transform, "assets/ship2.png", 40, 40, -90) /**/);
    auto& SelfDistruct(rocket.addComponent<SelfDistruct>());
}
void FirearmComponent::update(float mFT) {
    if (Game::event.type == SDL_KEYDOWN) {

        switch (Game::event.key.keysym.sym) {
        case(SDLK_SPACE):
            fire();
            break;
        default:
            break;
        }
    }
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
