#pragma once

#include <iostream>
#include <math.h>
#include "Vector2D.h"
#include "Game.h"

using namespace std;

struct TranformComponent : public Component{
    Vector2D position;
    Vector2D velocity;
    float angle;

public:

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
    TranformComponent( Vector2D *poz, float an) {
        position.x = poz->x;
        position.y = poz->y;

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

    float x() { return position.x; }
    float y() { return  position.y; }

	void setPos(float x, float y) {
        position.x = x;
        position.y = y;
	}
};


struct PlayerComponent : public Component {

    TranformComponent& transform;
    float plAc = 3;

    PlayerComponent(TranformComponent& _transform) : transform(_transform){


    }
    void update(float mFT) override {
        int mouseX, mouseY;

        if (Game::event.type == SDL_KEYDOWN) {

            switch (Game::event.key.keysym.sym) {
                 case SDLK_SPACE:
                     transform.velocity.x = 0;
                     transform.velocity.y = 0;

                     transform.position.x = 500;
                     transform.position.y = 500;

                     break;
            default:
                break;
            }

            if (Game::event.key.keysym.sym == SDLK_UP) {
                transform.velocity.x += mFT* plAc * cos(-transform.angle);
                transform.velocity.y += mFT * plAc * sin(-transform.angle);

            }
            else if (Game::event.key.keysym.sym == SDLK_DOWN) {
                transform.velocity.x += mFT * plAc * cos(-transform.angle + M_PI);
                transform.velocity.y += mFT * plAc * sin(-transform.angle + M_PI);
            }

            if (Game::event.key.keysym.sym == SDLK_LEFT) {
                transform.velocity.x += mFT * plAc * cos(transform.angle + M_PI / 2);
                transform.velocity.y += mFT * plAc * sin(transform.angle - M_PI / 2);
            }
            else if (Game::event.key.keysym.sym == SDLK_RIGHT ) {
                transform.velocity.x += mFT * plAc * cos(transform.angle - M_PI / 2);
                transform.velocity.y += mFT * plAc * sin(transform.angle + M_PI / 2);
            }
        }
        
        if(transform.velocity.x > 0)
        transform.velocity.x -= 0.5f;
        else
        transform.velocity.x += 0.5f;

        if(transform.velocity.y > 0)
        transform.velocity.y -= 0.5f;
        else
        transform.velocity.y += 0.5f ;


        transform.position.x += transform.velocity.x * mFT / 1000.0f;
        transform.position.y += transform.velocity.y * mFT / 1000.0f;

        if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
             transform.angle = atan2  (-mouseY + transform.position.y, mouseX - transform.position.x);

            /*
             std::cout <<"  ----------------------   " << endl;
             std::cout <<  - mouseY + transform.position.y <<"  "<< mouseX  -transform.position.x  << endl;
             std::cout << mouseY << "  " << mouseX << endl;
             std::cout << "Curent angle is:" << transform.angle * 180 / M_PI << endl;
             */
        }
    }

};


struct SimpleSprite :public Component {
private:
    TranformComponent& transform;
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    int orizontalSize, verticalSize;

public:
    SimpleSprite(TranformComponent& _transform, const char* texturesheet, int x, int y ) :transform(_transform) {

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
        destRect.h = 100;
        destRect.w = 100;
        destRect.x = x - destRect.h /2.0f;
        destRect.y = y - destRect.w / 2.0f;
    }

    void update(float mFT) override {
        //Draw the sprite at the position we desire
         destRect.x = transform.x() - destRect.h / 2.0f ;
         destRect.y = transform.y() - destRect.w / 2.0f;
    }

    /*int SDL_RenderCopyEx(SDL_Renderer*          renderer,
                     SDL_Texture*           texture,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip)
*/

    void draw() override
    {
        if (SDL_RenderCopyEx(Game::renderer, 
            objTexture,
            &srcRect,
            &destRect,
            90 - (transform.angle * 180 / M_PI ),
            NULL,
            SDL_FLIP_NONE
            ) != 0) {
            std::cout << SDL_GetError() << '\n';
        }
    }

};

