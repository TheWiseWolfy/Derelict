// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <cassert>
#include <SDL.h>

// Forward-declaration 
class Entity;
class Component;

//Tip de data ID si nr maxim de componente suportate de engine
using ComponentID = std::size_t; 
constexpr std::size_t maxComponents = 32;

//Pot sa ascund fuctii globale cu Internal, am nevoie doar sa primesc numere
namespace Internal{
    inline ComponentID getUniqueComponentID() { //se mai foloseste noexcept dar nu sunt sigur de ce
        static ComponentID lastID= 0 ;
        return lastID++;
    }
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    //Compile time assert
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    //Pentru ca e static, o sa atribuie un singur id pentru fiecare tip de template chemat, si apoi sa tot returneze acelasi numar
    //Ruleaza o singura data per tip de template nou
    static ComponentID typeID = Internal::getUniqueComponentID();
    return typeID;
}

class Component     
{
private:

public:
    Entity* parentEntity = nullptr; 
    
    virtual void update(float mFT) {}
    virtual void draw() {}
    
    virtual ~Component() {}

    Entity* getParentEntity(){
        return parentEntity;
    }
};

// Un "profil" al fiecarei antitati, o metoda rapida de a stoca ce entitate are ce componenta.
// O entitate cu un tranform ar trebui sa aiba doar "1 0 0 0..." 
using ComponentBitset = std::bitset<maxComponents>;

//!!! Am un vector elegant cu unique_ptr pe care il accesez regulat, dar daca vreau sa accesed obiectul cu 
//ID-ul 15, nu o sa creez 14 intervale nule, ca sa pun componenta relevanta pe locul 15. Alternativa e sa am 
//un vector parcurs regulat, si un array al carui rol e doar sa stocheze adrese in caz ca am nevoie de ele si 
//stiu doar ID-ul. Nu e elegant.
using ComponentArray = std::array<Component*, maxComponents>;

//Clasa de entitate suporta fuctionalitati de:
//Adaugi componente, verifici daca exista, le recuperezi
//Actualizez codul asociat tuturor componentelor
//Entitatea este inlaturata daca are alive = "false"

//Obiectul fundamental al jocului
class Entity
{
private:
    bool alive = true;  //entitatea incepe ca fiind "in viata"

    std::vector<std::unique_ptr<Component>> components;      //fiecare entitate incorporeaza un numar de componente
    std::array<Component*, 32> componentArray;

    ComponentBitset componentBitset;
public:

    Entity() {
    }

    // Odata ce componentele virtuale au fost suprascrise, putem interactiona cu ele pur si simplu prin 2 fuctii.
    void update(float mFT);
    void draw();

    // control lifetime entitate
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    //Pot vedea daca o componenta exista, wow
    template<typename T>
    inline bool hasComponent() const {
        return componentBitset[getComponentTypeID<T>()];
    }

   
    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs);

    template <typename T>
    T& getComponent() const;

};

//compiler-ul va determina numarul si tipul de argumente necesare pentru fiecare componenta si fuctia addComponent va putea
//chema constructorul corespunzator fiecarui tip de componenta

template<typename T, typename ...TArgs>
 T& Entity::addComponent(TArgs && ...mArgs) {

    T* c = new T(std::forward<TArgs>(mArgs)...);   //cream o componenta noua, de tipul cu care a fost chemat template-ul, iau apoi ii dam argumentele primite

    c->parentEntity = this;           //atasam "parintele componentei" ca intr-o lista inlantuita


    // c( un pointer "gol" ) va fi de acum sub controlul unui containter de tip "unuqie_ptr" care respecta principile RAII
    //cand entitatea este distrusa, components va fi terminat, si unuqie_ptr va elibera memoria corespunzator
    std::unique_ptr<Component> uPtr(c); //-V824
    components.emplace_back(std::move(uPtr)); //uPtr este mutata in loc

     //imformatie suplimentara pentru manipulare
    componentArray[getComponentTypeID<T>()] = c;
    componentBitset[getComponentTypeID<T>()] = true;

    //returnam referinta la componenta propiuzisa
    return *c;
}

 template<typename T>
  T& Entity::getComponent() const
 {
     //Entitatea trebuie sa existe
     assert(hasComponent<T>());

     //Ne uitam in array-ul auxiliar dupa ID, si gasim exact ce ne asteptam sa gasim
     auto ptr(componentArray[getComponentTypeID<T>()]);

     //Convertim implicit pointerul de tip component, in pointer la tipul T, si dereferientem.
     return *(static_cast<T*>(ptr) /**/);
 }
