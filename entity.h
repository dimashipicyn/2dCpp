#ifndef ENTITY_H
#define ENTITY_H

#include "vector.h"

#include <memory>

class Game;

class Entity
{
public:
    using ptr = std::shared_ptr<Entity>;
    
    Entity();
    virtual ~Entity();
    
    virtual void input(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;
    
    Vec2f get_position() const;
    Vec2f get_direction() const;
    
    void set_position(const Vec2f& pos);
    void set_direction(const Vec2f& dir);
    
    void translate(float tr);
    void rotate(float rot);

private:
    Vec2f position_;
    Vec2f direction_;
};

#endif
