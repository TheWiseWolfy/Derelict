#pragma once

#include <iostream>
#include <vector>
#include<SDL.h>
// Forward-declaration 
class Entity;

class Component
{
public:
    Entity* entity{ nullptr }; //initializer list

    virtual void update(float mFT) {}
    virtual void draw() {}

    virtual ~Component() {}
};


// Even if the `Entity` class may seem complex, conceptually it is
// very simple. Just think of an entity as a container for components,
// with syntatic sugar methods to quicky add/update/draw components.


class Entity
{
private:
    bool alive = true;  //entitatea incepe ca fiind "in viata"

    std::vector<std::unique_ptr<Component>> components;      //fiecare entitate incorporeaza un numar de componente
public:
    Entity() {

    }

    // Odata ce componentele virtuale au fost suprascrise, putem interactiona cu ele pur si simplu prin 2 fuctii.
    void update(float mFT);
    void draw();

   
    // control lifetime entitate
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    //compiler-ul va determina numarul si tipul de argumente necesare pentru fiecare componenta si fuctia addComponent va putea
    //chema constructorul corespunzator fiecarui tip de componenta
    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) { 
       

        T* c = new T(std::forward<TArgs>(mArgs)...);   //cream o componenta noua, de tipul cu care a fost chemat template-ul, iau apoi ii dam argumentele primite

        c->entity = this;
        //atasam "parintele componentei" ca intr-o lista inlantuita

        // c( un pointer "gol" ) va fi de acum sub controlul unui containter de tip "unuqie_ptr" care respecta principile RAII
        //cand entitatea este distrusa, components va fi terminat, si unuqie_ptr va elibera memoria corespunzator
        std::unique_ptr<Component> uPtr(c);
        components.emplace_back(std::move(uPtr)); //uPtr este mutata in loc

        //returnam referinta la componenta propiuzisa
        return *c;

    }
};


