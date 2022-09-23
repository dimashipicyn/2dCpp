#ifndef PHYSICS_H
#define PHYSICS_H

#include "vector.h"
#include "entity.h"

#include <vector>
#include <memory>

namespace physics {

enum class ColliderType
{
	AABB,
	CIRCLE
};

struct AABB
{
    Vec2f pos;
    Vec2f size;
};

struct Circle
{
    Vec2f pos;
	float radius;
};

class Collider
{
public:
	Collider(const AABB& aabb);
	Collider(const Circle& circle);
	~Collider();

	bool has_intersection(const Collider& other) const;

private:
	bool has_intersection(const AABB& aabb_1, const AABB& aabb_2) const;
	bool has_intersection(const Circle& circle_1, const Circle& circle_2) const;
	bool has_intersection(const AABB& aabb, const Circle& circle) const;

	ColliderType type_;
	union {
		AABB aabb_;
		Circle circle_;
	};
};

enum class Layer : int {
	LAYER_0 = 0,
	LAYER_1 = 1,
	LAYER_2 = 2,
	LAYER_3 = 4
};

class Body
{
public:
	using ptr = std::shared_ptr<Body>;

	Body(const AABB& aabb);
	Body(const Circle& circle);
	~Body();

	void set_owner(Entity::ptr owner);
	void set_position(const Vec2f& pos);
	void set_direction(const Vec2f& dir);
	void set_layer(Layer layer);
	void set_velocity(float vel);
	void set_acceleration(float acc);
	void set_mass(float mass);
	void set_active(bool active);

	Entity::ptr get_owner() const;
	Collider get_collider() const;
	Vec2f get_position() const;
	Vec2f get_direction() const;
	Layer get_layer() const;
	float get_velocity() const;
	float get_acceleration() const;
	float get_mass() const;
	bool is_active() const;

private:
    Entity::weak_ptr	owner_;
	Collider			collider_;
	Vec2f				position_;
	Vec2f           	direction_;
	Layer				layer_;
    float   	        velocity_;
	float				acceleration_;
	float				mass_;
	bool            	is_active_;
};

class World
{
public:
	using ptr = std::unique_ptr<World>;
	
	World(float gravity);
	~World();


    void step();

	void add_body(Body::ptr body, bool dynamic);
	void remove_body(Body::ptr body);
	
    //bool has_intersection(const AABB& aabb);
    
    //Physic_body* create_body(const Physic_body_def& def);
	//void remove_body(const Physic_body* body);

private:
	void process_collision(Body& one, Body& two);
	void resolve_collision(Body& one, Body& two);

    
private:
    std::vector<Body::ptr> static_body_;
    std::vector<Body::ptr> dynamic_body_;
	float gravity_;
};

}

#endif
