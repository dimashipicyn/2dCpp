#ifndef ENTITY_H
#define ENTITY_H

#include "vector.h"
#include "physics.h"

#include <memory>

class Game;

class Entity
{
public:
    using ptr = std::shared_ptr<Entity>;
	using weak_ptr = std::weak_ptr<Entity>;
	using body_ptr = physics::Body::ptr;
    
    Entity();
    virtual ~Entity();
    
    virtual void input(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;
    
    Vec2f get_position() const;
    Vec2f get_direction() const;
	body_ptr get_body();
    
    void set_position(const Vec2f& pos);
    void set_direction(const Vec2f& dir);
	void set_body(body_ptr body);
    
    void translate(float tr);
    void rotate(float rot);

private:
	body_ptr body_;
};

#endif
