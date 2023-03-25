#ifndef PHYSICS_H
#define PHYSICS_H

#include "Common.hpp"

#include <glm/vec2.hpp>

#include <vector>
#include <memory>
#include <functional>

//namespace physics {

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
	glm::vec2 get_position() const;
	void set_position(const glm::vec2& pos);

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

enum BitMask
{
	BIT_0  = 1 << 0,
	BIT_1  = 1 << 1,
	BIT_2  = 1 << 2,
	BIT_3  = 1 << 3,
	BIT_4  = 1 << 4,
	BIT_5  = 1 << 5,
	BIT_6  = 1 << 6,
	BIT_7  = 1 << 7,
};

class Body;
using BodyPtr = Ptr<Body>;

using OnCollisionCallback = std::function<void(const Body& one, const Body& two)>;

class Body
{
public:
	friend class Physics;

	Body();
	Body(const AABB& aabb);
	Body(const Circle& circle);
	~Body();

	void set_position(const glm::vec2& pos);
	void set_direction(const glm::vec2& dir);
	void set_bit_mask(BitMask bit);
	void set_velocity(float vel);
	void set_mass(float mass);
	void set_enable(bool enable);

	void on_collision(OnCollisionCallback callback);

	BitMask get_bit_mask() const;
	Collider get_collider() const;
	glm::vec2 get_position() const;
	glm::vec2 get_direction() const;
	float get_velocity() const;
	float get_mass() const;

	bool is_enabled() const;

private:
	OnCollisionCallback	callback_;
	Collider			collider_;
	glm::vec2			position_;
	glm::vec2           direction_;
	BitMask				bitmask_;
    float   	        velocity_;
	float				mass_;
	bool            	is_enabled_;
};

class Physics
{
public:
	Physics(float gravity);
	~Physics();

    void step(float dt);

	Body* create(const AABB& aabb, bool dynamic = false);
	Body* create(const Circle& circle, bool dynamic = false);
	void remove(const Body* body);

private:
	void process_collision(Body& one, Body& two);
	void resolve_collision(Body& one, Body& two);
    
private:
    std::vector<BodyPtr> static_body_;
    std::vector<BodyPtr> dynamic_body_;
	float gravity_;
};

//}

#endif
