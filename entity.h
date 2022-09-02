#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

class Game;

class Entity
{
public:
    using ptr = std::shared_ptr<Entity>;
    
    virtual ~Entity();
    
    virtual void input(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;
};

#endif
