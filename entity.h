#ifndef ENTITY_H
#define ENTITY_H

#include "vector.h"

#include <memory>

class Game;

struct Transform
{
	Vec2f position = {0,0};
	Vec2f rotation = {0,1};

	void translate(Vec2f tr) {
		position.add(tr);
	}

	void rotate(Vec2f rot) {
		rotation.add(rot);
		rotation.normalize();
	}
};

class Entity
{
public:
    using ptr = std::shared_ptr<Entity>;
    
    virtual ~Entity();
    
    virtual void input(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;

	Transform transform;

private:
	
};

#endif
