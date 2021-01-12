#include <iostream>
#include <math.h>
#include <string>

#include "Game.h"
#include "Components.h"
#include "Coliders.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "LevelManager.h"

//#define firerate 100
#define regen 5000
#define thrusterSoundFreq 600
#define minimumEnemyDistance 300.0f
#define maximumEnemyDistance 700.0f
#define enemyFireDistance 750.0f
#define enemyFireRate 1000

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

    //Actualizam pozitia un fuctie de viteza curenta
    position.x += velocity.x * mFT / 1000.0f;
    position.y += velocity.y * mFT / 1000.0f;

    //Aici cream limitele nivelului
    if (position.x <= 0)
        velocity.x +=100 ;

    if (position.x >= Level::levelWidth)
        velocity.x -= 100;

    if (position.y <= 0)
        velocity.y += 100;

    if (position.y >= Level::levelHeigh)
        velocity.y -= 100;

    //Actualizam niste vectori cu directii relevante pentru transform
    forward.x = (float)cos(angle + M_PI);
    forward.y = (float)sin(angle + M_PI);

    left.x = (float)cos(angle + M_PI / 2);
    left.y = (float)sin(angle + M_PI / 2);

    //O limita de viteza
    if (velocity.x < -1300.0f) {
        velocity.x += 0.3f;
    }
    else if (velocity.x > 1300.0f){
        velocity.x -= 0.3f;
    }

    if (velocity.y < -1300.0f){
        velocity.y += 0.3f;
    }
    else if (velocity.y > 1300.0f){
        velocity.y -= 0.3f;
    }
}

//!COLIDER
Collider::Collider(Transform& _transform, Wireframe _vecModel) : transform(_transform) {

    vecModel = _vecModel;
    vecModelinWolrd = _vecModel;

}

//!Fuctia asta e o forma simpla de a calcula coliziuni si a aplica niste forte normale.
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
//!Aici contextualizam forma colider-ului in lume printr-o rotatie si o translatie
void Collider::update(float mFT) {

    // Rotate
    for (size_t i = 0; i < vecModelinWolrd.size(); i++) {
        vecModelinWolrd[i].first = vecModel[i].first * cos(transform.angle) - vecModel[i].second * sin(transform.angle);
        vecModelinWolrd[i].second = vecModel[i].first * sin(transform.angle) + vecModel[i].second * cos(transform.angle);
    }
    //Translate
    for (size_t i = 0; i < vecModel.size(); ++i) {
        vecModelinWolrd[i].first = vecModelinWolrd[i].first + transform.position.x;
        vecModelinWolrd[i].second = vecModelinWolrd[i].second + transform.position.y;
    }

}
//!Nimic de desenat
void Collider::draw()
{

}

//!PLAYER COMPONENT
PlayerComponent::PlayerComponent(Transform& _transform) : transform(_transform) {}

//!Controalele jocului
void PlayerComponent::update(float mFT) {

    //Set up
    Entity* player = this->getParentEntity();
    int mouseX, mouseY;

    //Ne asiguram ca avem cu ce lucra
    assert(player->hasComponent<FirearmComponent>());

    //Un interval de timp la care tragem, ne regeneram si folosim un efect sonor pentru motoare
    soundCounter += mFT;
    regenCounter += mFT;
    fireCounter += mFT;

    //Keyboard control
    if (Game::event.type == SDL_KEYDOWN) {

        //Aici sunt controalele principale de directie
        if (Game::event.key.keysym.sym == SDLK_UP) {
            transform.velocity += mFT * (playerForce / transform.mass) * transform.forward;

            if (soundCounter > thrusterSoundFreq) {
                SoundManager::Instance()->PlaySound("assets/RocketSound.wav", 0);
                soundCounter = 0;
            }
        }
        else if (Game::event.key.keysym.sym == SDLK_DOWN) {
            transform.velocity += mFT * -(playerForce / transform.mass) * transform.forward;
            
            if (soundCounter > thrusterSoundFreq) {
                SoundManager::Instance()->PlaySound("assets/RocketSound.wav", 0);
                soundCounter = 0;
            }
        }
        if (Game::event.key.keysym.sym == SDLK_LEFT) {
            transform.velocity += mFT * (playerForce / transform.mass) * transform.left;
            
            if (soundCounter > thrusterSoundFreq) {
                SoundManager::Instance()->PlaySound("assets/RocketSound.wav", 0);
                soundCounter = 0;
            }
        }
        else if (Game::event.key.keysym.sym == SDLK_RIGHT) {
            transform.velocity += mFT * -(playerForce / transform.mass) * transform.left;

            if (soundCounter > thrusterSoundFreq) {
                SoundManager::Instance()->PlaySound("assets/RocketSound.wav", 0);
                soundCounter = 0;
            }
        }
         if (Game::event.key.keysym.sym == SDLK_SPACE) {
             transform.velocity += transform.velocity * (-1 / 10.0f);

        }
    }

    //Recuperam componenta firearm
    FirearmComponent* firearm = &(player->getComponent<FirearmComponent>());

    //Control mouse
    if(SDL_GetMouseState(&mouseX, &mouseY) && SDL_BUTTON(SDL_BUTTON_LEFT)) {

       /*
       std::cout <<"  ----------------------   " << std::endl;
       std::cout << mouseY << "  " << mouseX << std::endl;
       std::cout << "Ship position: X: " << transform.position.x << "Y: " << transform.position.y << " \n";
       std::cout <<"Mouse game position: X: "<< gameMouse.x <<"Y: "<< gameMouse.y << endl;
       std::cout << transform.position.x - gameMouse.x << "  " << transform.position.y - gameMouse.y << endl;
       std::cout << "Curent angle is:" << transform.angle << "\n";  
       std::cout << "Curent desired angle is:" << derisedAngle << "\n"; 
        */
         
        //tragem la o anumita rata
       if (fireCounter >= firerate) {
           firearm->fire();
           fireCounter = 0;
       }
    }

    Vector2D gameMouse = Level::screenSpaceToGameSpace(mouseX, mouseY);
    float derisedAngle = atan2(transform.position.y - gameMouse.y, transform.position.x - gameMouse.x);
    transform.angle = derisedAngle;
    
    //Camera ramane centrata pe jucator
    Level::camera_position.x = -transform.position.x + Level::camera_size.x / 2;
    Level::camera_position.y = -transform.position.y + Level::camera_size.y / 2;

    //Regenerare pasiva
    if (life < maxLife) {
        if (regenCounter >= regen) {
            life++;
            regenCounter = 0;
        }
    }
}
//!Some helpful debuging
void PlayerComponent::draw(){
    //optional debug line
    //SDL_RenderDrawLine(Game::renderer,
    //    transform.position.x + Level::camera_position.x,
    //    transform.position.y + Level::camera_position.y,
    //    transform.position.x + 100 * transform.forward.x + Level::camera_position.x,
    //    transform.position.y + 100 * transform.forward.y + Level::camera_position.y
    //);
}
//!Fuctie chemata la lovitura cu un proiectil
void PlayerComponent::onHit() {
    life -= 1;
}

//!ENEMY AI COMPONENT
EnemyComponent::EnemyComponent(const Entity& _player, Transform& _transform) : player(_player), transform(_transform)
{
}
void EnemyComponent::update(float mFT){

    //Set up
    Entity* enemy = this->getParentEntity();
    auto& playerTransform = player.getComponent<Transform>();

    //Ne asiguram ca avem firearm
    assert(enemy->hasComponent<FirearmComponent>() );

    //Inamicul se uita mareu la jucator
    Vector2D playerPoz = playerTransform.position;
    float derisedAngle = atan2(transform.position.y - playerPoz.y, transform.position.x - playerPoz.x);
    transform.angle = derisedAngle;

    //Pastram o anumita distanta fata de jucator
    Vector2D player_enemy = transform.position - playerTransform.position;
    float distance = player_enemy.getMagnitude();

    if (distance > minimumEnemyDistance) {
        transform.velocity += mFT * (1 / transform.mass) * transform.forward;
    }
    else if (distance < maximumEnemyDistance) {
        transform.velocity += mFT * -(1 / transform.mass) * transform.forward;
    }

    //Cat este in raza, inamicul va trage in continuu
    FirearmComponent* firearm = &(enemy->getComponent<FirearmComponent>() );
    counter += mFT;
    if (counter >= enemyFireRate && distance < enemyFireDistance) {
        firearm->fire();
        counter = 0;
    }

    //Daca ramane fara viata, acesta va murii si va actualiza cifrele relevante
    if (life <= 0) {
      Level::activeEnemies--;
      Game::score++;
      enemy->destroy();

      SoundManager::Instance()->PlaySound("assets/explosion.wav", 0);     //sunet de explozie
    }
}
void EnemyComponent::draw(){

}
void EnemyComponent::onHit(){
    life -= 1;
}

//!SIMPLE SPRITE
SimpleSprite::SimpleSprite(Transform& _transform, const char* texturesheet, int h, int w, int rotation) :transform(_transform) {

    objTexture = AssetManager::Instance()->GetTexture(texturesheet);

    //Stocam marimea texturii
    orizontalSize = 0;
    verticalSize = 0;

    SDL_QueryTexture(objTexture, NULL, NULL, &orizontalSize, &verticalSize);

    //Aici definim sprit-ul ca fiind intreaga imagine
    srcRect.h = verticalSize;
    srcRect.w =  orizontalSize;
    srcRect.x = 0;
    srcRect.y = 0;

    //Definim marimea pe ecran
    destRect.h = h;
    destRect.w = w;

    //Rotatia
    spriteRotation = rotation;
}
SimpleSprite::~SimpleSprite(){
}
void SimpleSprite::update(float mFT) {
    //Desenam  obiectul in fuctie de pozitia lui relativ la camera.
    destRect.x = Level::camera_position.x + transform.x() - destRect.h / 2.0f;
    destRect.y = Level::camera_position.y + transform.y() - destRect.w / 2.0f;
}
void SimpleSprite::draw() {
    //desenarea propiuzisa
    if (SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect, ( ( (double)transform.angle * 180.0f) / M_PI + spriteRotation),
        NULL, SDL_FLIP_NONE) != 0)
    {
        std::cout << "Simple spirte draw error: "<< SDL_GetError()  << '\n';
    }
}

//!STATIC SPRITE
StaticSprite::StaticSprite( const char* texturesheet, int _x, int _y,int h, int w,float _scroll,int rotation) {
    objTexture = AssetManager::Instance()->GetTexture(texturesheet);

    orizontalSize = 0;
    verticalSize = 0;

    SDL_QueryTexture(objTexture, NULL, NULL, &orizontalSize, &verticalSize);

    srcRect.h = verticalSize;
    srcRect.w = orizontalSize;
    srcRect.x = 0;
    srcRect.y = 0;

    //Plasam obiectul static pe ecran
    destRect.h = h;
    destRect.w = w;
    destRect.x = 0;
    destRect.y = 0;

    x = _x;
    y= _y;
    spriteRotation = rotation;
    scrollRate = _scroll;
}
StaticSprite::~StaticSprite(){
}
void StaticSprite::update(float mFT){
 
    //Putem crea un efect de paralax daca miscam obiectul relativ la camera dar impartit la o anumita rata, obiectele cu rata mai mare o sa para mai departe
    if (scrollRate != 0){
    destRect.x = Level::camera_position.x / scrollRate + x ;
    destRect.y = Level::camera_position.y / scrollRate + y;
    }
}
void StaticSprite::draw(){
    if (SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect,spriteRotation,
        NULL, SDL_FLIP_NONE) != 0)
    {
        std::cout << SDL_GetError() << '\n';
    }  
}

//!FIREARM COMPONENT
FirearmComponent::FirearmComponent(Transform& _transform) : transform(_transform) {}
//!Instantiem un proiectil cu un colider potrivit, la pozitia si cu viteza potrivita
void FirearmComponent::fire() {

   Wireframe vecModelShip = {
   {-25.0,0},
   {25.0f,-25.0f},
   {25.0f,25.0f}
   };

   //"comandam" obiectul ca acesta sa fie introdus inainte de urmatorul cadru
   auto& rocket(Game::entityManager.rezerveEntity());
    Vector2D poz(transform.position.x, transform.position.y);
    auto& rocket_transform(rocket.addComponent<Transform>(poz + transform.forward * 80, 1000 * transform.forward + transform.velocity, transform.angle ,3)/**/);
    auto& rochet_colider(rocket.addComponent<ProjectileCollider>(rocket_transform, vecModelShip));
    auto& rocket_transform_sprite(rocket.addComponent<SimpleSprite>(rocket_transform, "assets/bullet.png", 40, 40, 180) /**/);
    auto& SelfDistruct(rocket.addComponent<SelfDistruct>());

    SoundManager::Instance()->PlaySound("assets/laser.wav", 0);   //pew pew
}
void FirearmComponent::update(float mFT) {

}
void FirearmComponent::draw()
{

}

//!SELF DISTRUCT COMPONENT
SelfDistruct::SelfDistruct(){
}
void SelfDistruct::update(float mFT){
    counter += mFT;
    if (counter >= 1000) this->getParentEntity()->destroy();

}
void SelfDistruct::draw()
{
}
