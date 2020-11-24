#pragma once
#include "Vector2D.h"
#include "Game.h"

struct TranformComponent : public Component{
private:
	float xPos;
	float yPos;

public:
    Vector2D position;
    Vector2D velocity;

	TranformComponent() {
        position.x = 0;
        position.y = 0;

        velocity.x = 0;
        velocity.y = 0;
	}

	TranformComponent(float x, float y) {
        position.x = x;
        position.y = y;

        velocity.x = 0;
        velocity.y = 0;
	}

    TranformComponent(float x, float y,float vx,float vy) {
        position.x = x;
        position.y = y;

        velocity.x = vx;
        velocity.y = vy;
    } 

    TranformComponent(const Vector2D& poz) {
        position.x = poz.x;
        position.y = poz.y;

        velocity.x = 0;
        velocity.y = 0;
    }

    TranformComponent(const Vector2D& poz, const Vector2D& vel) {
        position.x = poz.x;
        position.y = poz.y;

        velocity.x = vel.x;
        velocity.y = vel.y;
    }

    float x() { return position.x; }
    float y() { return  position.y; }

	void setPos(float x, float y) {
        position.x = x;
        position.y = y;
	}

    void update(float mFT) override {
        if (Game::event.type == SDL_KEYDOWN) {
           
            switch (Game::event.key.keysym.sym){
            case SDLK_ESCAPE:
                entity->destroy();
                break;
            default:
                break;
            }

            if (Game::event.key.keysym.sym == SDLK_UP ){
                velocity.y -= 1;
            }
            else if (Game::event.key.keysym.sym == SDLK_DOWN) {
                velocity.y += 1;
            }
                     
            if (Game::event.key.keysym.sym == SDLK_RIGHT) {
                velocity.x += 1;
            }
            else if (Game::event.key.keysym.sym == SDLK_LEFT) {
                velocity.x -= 1;
            }          
       }

        position.x += velocity.x * mFT/1000.0f;
        position.y += velocity.y * mFT / 1000.0f;

    }
};

struct CounterComponent :public Component {
public:
    float counter;
    void update(float mFT) override
    {
        counter += mFT;
        std::cout << counter << std::endl;
    }
};


struct KillComponent :public  Component {
    CounterComponent& cCounter;

    KillComponent(CounterComponent& mCounterComponent): cCounter(mCounterComponent)
    {
    }

    void update(float mFT) override
    {
        if (cCounter.counter >= 5000) entity->destroy();
    }
};

struct SimpleSprite :public Component {
    TranformComponent& transform ;
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    SDL_Renderer* renderer;


    SimpleSprite(TranformComponent& _transform,const char* texturesheet, int x, int y, SDL_Renderer* _renderer) :transform(_transform) {
        
        renderer = _renderer;
        objTexture = TextureManager::LoadTexture(texturesheet, renderer);

        srcRect.h = 500;
        srcRect.w = 500;
        srcRect.x = 0;
        srcRect.y = 0;

        destRect.h = 500;
        destRect.w = 500;
       
    }

    void update(float mFT) override {

        destRect.x = transform.x();
        destRect.y = transform.y();
    }

    void draw() override
    {
        if (SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect) != 0) {
            std::cout << SDL_GetError() << '\n';
        }
    }

};
