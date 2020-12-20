#pragma once

#include <iostream>
#include <math.h>
#include <string>

#include "Vector2D.h"
#include "Game.h"

//More co

using namespace std;

struct TranformComponent : public Component{
    //Position is game space
    Vector2D position;
    float angle;

public:
    Vector2D velocity;

    Vector2D forward;  //A vector that always points forward 
    Vector2D left; 

    TranformComponent() {
        position.x = 0;
        position.y = 0;

        velocity.x = 0;
        velocity.y = 0;

        angle = 0;
    }
    TranformComponent(float x, float y, float an) {
        position.x = x;
        position.y = y;

        velocity.x = 0;
        velocity.y = 0;

        angle = an;
    }
    TranformComponent(float x, float y, float vx, float vy, float an) {
        position.x = x;
        position.y = y;

        velocity.x = vx;
        velocity.y = vy;

        angle = an;
    }
    TranformComponent(const Vector2D& poz, float an) {
        position.x = poz.x;
        position.y = poz.y;

        velocity.x = 0;
        velocity.y = 0;

        angle = an;
    }
    TranformComponent(const Vector2D& poz, const Vector2D& vel, float an) {
        position.x = poz.x;
        position.y = poz.y;

        velocity.x = vel.x;
        velocity.y = vel.y;

        angle = an;
    }

    TranformComponent( Vector2D *poz, float an) {
        position.x = poz->x;
        position.y = poz->y;

        velocity.x = 0;
        velocity.y = 0;

        angle = an;
    }
    TranformComponent(Vector2D* poz, Vector2D* vel ,float an) {
        position.x = poz->x;
        position.y = poz->y;

        velocity.x = vel->x;
        velocity.y = vel->y;
        angle = an;
    }

    float x() { return position.x; }
    float y() { return  position.y; }

	void setPos(float x, float y) {
        position.x = x;
        position.y = y;
	}

    void update(float mFT) override {

        //Update position based on velocity 
       position.x += velocity.x * mFT / 1000.0f;
       position.y += velocity.y * mFT / 1000.0f;



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
        forward.x =  cos(angle + M_PI );
        forward.y =  sin(angle + M_PI ) ;

        left.x = cos(angle + M_PI/2);
        left.y = sin(angle + M_PI/2);
    }
};

struct PlayerComponent : public Component {

    TranformComponent& transform;
    float plAc = 1.3;

    PlayerComponent(TranformComponent& _transform) : transform(_transform){}

    void update(float mFT) override {
        int mouseX, mouseY;

        //cout << transform.forward * 100 << "\n";

        if (Game::event.type == SDL_KEYDOWN) {
            switch (Game::event.key.keysym.sym) {
            default:
                break;
            }

            //One idea for the controls, might redu later
            if (Game::event.key.keysym.sym == SDLK_UP) {
                 transform.velocity += mFT * plAc *  transform.forward ;
            }
            else if (Game::event.key.keysym.sym == SDLK_DOWN) {
                transform.velocity += mFT * -plAc * transform.forward;
            }

            if (Game::event.key.keysym.sym == SDLK_LEFT) {
                transform.velocity += mFT * plAc * transform.left;
            }
            else if (Game::event.key.keysym.sym == SDLK_RIGHT ) {
                transform.velocity += mFT * -plAc * transform.left;

            }
        }

        //To redo
        if(transform.velocity.x > 0)
            transform.velocity.x -= 0.5f;
        else
            transform.velocity.x += 0.5f;

        if(transform.velocity.y > 0)
            transform.velocity.y -= 0.5f;
        else
            transform.velocity.y += 0.5f ;

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
        Level::camera_position.x = -transform.position.x + Level::camera_size.x/2;
        Level::camera_position.y = -transform.position.y + Level::camera_size.y/2;
    }


    void draw() override
    {
        //optional debug line
        SDL_RenderDrawLine(Game::renderer,
            transform.position.x + Level::camera_position.x,
            transform.position.y + Level::camera_position.y,
            transform.position.x+ 100 * transform.forward.x + Level::camera_position.x,
            transform.position.y+ 100 * transform.forward.y + Level::camera_position.y
        );
    }
};

struct SimpleSprite :public Component {
private:
    TranformComponent& transform;
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    int orizontalSize, verticalSize;
    int spriteRotation;

public:
    SimpleSprite(TranformComponent& _transform, const char* texturesheet, int h, int w,int rotation ) :transform(_transform) {

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

    void update(float mFT) override {
        //Draw the sprite at the position we desire

         destRect.x = Level::camera_position.x + transform.x() - destRect.h / 2.0f ;
         destRect.y = Level::camera_position.y + transform.y() - destRect.w / 2.0f;
    }

    void draw() override{
        //render in window space
        if (SDL_RenderCopyEx(Game::renderer, objTexture,&srcRect,&destRect,( /**/(transform.angle * 180) / M_PI + spriteRotation),
            NULL,SDL_FLIP_NONE) != 0) 
        {
            std::cout << SDL_GetError() << '\n';
        }
    }

};

struct RocketLogic : public Component {


};

struct FirearmComponent : public Component {

    TranformComponent& transform;

    FirearmComponent(TranformComponent& _transform) : transform(_transform) {}


    void fire() {
        auto& rocket(Game::entityManager.rezerveEntity() );

        Vector2D poz(transform.position.x, transform.position.y);
        auto& rocket_transform(rocket.addComponent<TranformComponent>(poz, 1000 * transform.forward + transform.velocity, transform.angle - M_PI /2)/**/);
        auto& rocket_transform_sprite(rocket.addComponent<SimpleSprite>(rocket_transform, "assets/ship2.png", 40, 40, 0) /**/);
    }

    void update(float mFT) override {
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

    void draw() override
    {

    }
};
