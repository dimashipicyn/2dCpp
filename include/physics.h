#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/vec2.hpp>

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
	AABB();
	AABB(float x, float y, float w, float h);
    glm::vec2 pos;
    glm::vec2 size;
};

struct Circle
{
	Circle();
	Circle(float x, float y, float r);
    glm::vec2 pos;
	float radius;
};

class Collider
{
public:
	Collider(const AABB& aabb);
	Collider(const Circle& circle);
	~Collider();

	bool has_intersection(const Collider& other) const;
	 get_position() const;
	void set_position(const Vec2f& pos);

	const AABB& get_aabb() const;
	const Circle& get_circle() const;

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

class World;

class Body
{
public:
	using ptr = std::shared_ptr<Body>;
	using entity_ptr = std::shared_ptr<Entity>;
	using entity_weak_ptr = std::weak_ptr<Entity>;
	using vector_collisions = std::vector<entity_weak_ptr>;

	friend class World;

	Body();
	Body(const AABB& aabb);
	Body(const Circle& circle);
	~Body();

	bool has_collision() const;

	void set_owner(entity_ptr owner);
	void set_position(const Vec2f& pos);
	void set_direction(const Vec2f& dir);
	void set_layer(Layer layer);
	void set_velocity(float vel);
	void set_acceleration(float acc);
	void set_mass(float mass);
	void set_active(bool active);

	entity_ptr get_owner() const;
	vector_collisions get_collisions() const;
	Collider get_collider() const;
	Vec2f get_position() const;
	Vec2f get_direction() const;
	Layer get_layer() const;
	float get_velocity() const;
	float get_acceleration() const;
	float get_mass() const;
	bool is_active() const;

private:
    entity_weak_ptr		owner_;
	vector_collisions	collisions_;
	Collider			collider_;
	Vec2f           	direction_;
	Layer				layer_;
    float   	        velocity_;
	float				acceleration_;
	float				mass_;
	bool            	is_active_;
	bool				has_collision_;
};

class World
{
public:
	using ptr = std::unique_ptr<World>;
	
	World(float gravity);
	~World();


    void step(float dt);

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
