#include "physics.h"
#include "entity.h"

#include <stdlib.h>
#include <SDL.h>

namespace physics {

World::World(float gravity)
	: static_body_()
	, dynamic_body_()
	, gravity_(gravity)
{
}

World::~World() {
	static_body_.clear();
	dynamic_body_.clear();
}

void World::resolve_collision(Body& one, Body& two)
{
	one.set_position(one.get_position().add(-one.get_direction().scalar(1000/60.0)));
	one.set_velocity(0);
	one.has_collision_ = true;

	two.set_position(two.get_position().add(-two.get_direction().scalar(1000/60.0)));
	two.set_velocity(0);
	two.has_collision_ = true;
//	auto aabb_a = one.get_collider();
//	auto aabb_b = two.get_collider();
//
//	if ((a.pos.x + a.size.x) < (b.pos.x + b.size.x * 0.5)) {
//		a.pos.x = b.pos.x + a.size.x;
//	}
//	else {
//		a.pos.x = b.pos.x + b.size.x;
//	}
//
//    if ((a.pos.y + a.size.y) < (b.pos.y + b.size.y * 0.5)) {
//		a.pos.y = b.pos.y + a.size.y;
//    }
//	else {
//		a.pos.y = b.pos.y + b.size.y;
//	}
}

void World::process_collision(Body& one, Body& two) {
	int layer_one = static_cast<int>(one.get_layer());
	int layer_two = static_cast<int>(two.get_layer());

	if (!(layer_one & layer_two)) {
		return;
	}

	bool has_intersect = one.get_collider().has_intersection(two.get_collider());
	if (has_intersect) {
		//d_body->contacted_bodyes.push_back(s_body.get());
		resolve_collision(one, two);
	}
}

void World::step(float dt)
{
    for (auto& one : dynamic_body_)
    {
        if (!one->is_active()) {
            continue;
        }
		one->has_collision_ = false;

        for (auto& two : static_body_)
        {
            if (!two->is_active()) {
                continue;
            }
			process_collision(*one, *two);
        }

        for (auto& two : dynamic_body_)
        {
            if (!two->is_active() || (one.get() == two.get())) {
                continue;
            }
			process_collision(*one, *two);
        }
    }
}

void World::add_body(Body::ptr body, bool dynamic) {
	if (dynamic) {
		dynamic_body_.push_back(body);
	}
	else {
		static_body_.push_back(body);
	}
}

void World::remove_body(Body::ptr body) {
	auto predicat = [&body](const auto& other)
	{
		return body.get() == other.get();
	};
	dynamic_body_.erase(std::remove_if(dynamic_body_.begin(), dynamic_body_.end(), predicat), dynamic_body_.end());
	static_body_.erase(std::remove_if(static_body_.begin(), static_body_.end(), predicat), static_body_.end());
}

AABB::AABB()
	: pos()
	, size()
{

}

AABB::AABB(float x, float y, float w, float h)
	: pos(x, y)
	, size(w, h)
{

}

Circle::Circle()
	: pos()
	, radius(0)
{

}

Circle::Circle(float x, float y, float r)
	: pos(x, y)
	, radius(r)
{

}

bool Collider::has_intersection(const Collider& other) const {
	if (type_ == ColliderType::AABB) {
		if (other.type_ == ColliderType::AABB) {
			return has_intersection(aabb_, other.aabb_);
		}
		else {
			return has_intersection(aabb_, other.circle_);
		}
	}
	else {
		if (other.type_ == ColliderType::AABB) {
			return has_intersection(other.aabb_, circle_);
		}
		else {
			return has_intersection(circle_, other.circle_);
		}
	}
}

Collider::Collider(const AABB& aabb)
	: type_(ColliderType::AABB)
	, aabb_(aabb)
{

}

Collider::Collider(const Circle& circle)
	: type_(ColliderType::CIRCLE)
	, circle_(circle)
{

}

Collider::~Collider() {

}

bool Collider::has_intersection(const AABB& aabb_1, const AABB& aabb_2) const {
	if ((aabb_1.pos.x + aabb_1.size.x) < aabb_2.pos.x || (aabb_2.pos.x + aabb_2.size.x) < aabb_1.pos.x) {
        return false;
    }
    if ((aabb_1.pos.y + aabb_1.size.y) < aabb_2.pos.y || (aabb_2.pos.y + aabb_2.size.y) < aabb_1.pos.y) {
        return false;
    }
    return true;
}

bool Collider::has_intersection(const Circle& circle_1, const Circle& circle_2) const {
	if ((circle_1.radius + circle_2.radius) <= circle_1.pos.sub(circle_2.pos).length()) {
		return true;
	}
	return false;
}

bool Collider::has_intersection(const AABB& aabb, const Circle& circle) const {
	return false;
}

Vec2f Collider::get_position() const {
	if (type_ == ColliderType::CIRCLE) {
		return circle_.pos;
	}
	else {
		auto pos = aabb_.pos;
		auto size = aabb_.size;
		return Vec2f(pos.x + (size.x / 2), pos.y + (size.y / 2));
	}
}

void Collider::set_position(const Vec2f& pos) {
	if (type_ == ColliderType::CIRCLE) {
		circle_.pos = pos;
	}
	else {
		auto size = aabb_.size;
		aabb_.pos = Vec2f(pos.x - (size.x / 2), pos.y - (size.y / 2));
	}
}

Body::Body()
	: owner_()
	, collider_(AABB())
	, direction_()
	, layer_(Layer::LAYER_0)
	, velocity_(0)
	, acceleration_(0)
	, mass_(0)
	, is_active_(false)
{

}

Body::Body(const AABB& aabb)
	: owner_()
	, collider_(aabb)
	, direction_()
	, layer_(Layer::LAYER_0)
	, velocity_(0)
	, acceleration_(0)
	, mass_(0)
	, is_active_(false)
{

}

Body::Body(const Circle& circle)
	: owner_()
	, collider_(circle)
	, direction_()
	, layer_(Layer::LAYER_0)
	, velocity_(0)
	, acceleration_(0)
	, mass_(0)
	, is_active_(false)
{

}

Body::~Body()
{

}

bool Body::has_collision() const {
	return has_collision_;
}

void Body::set_owner(entity_ptr owner) {
	owner_ = std::move(owner);
}

void Body::set_position(const Vec2f& pos) {
	collider_.set_position(pos);
}

void Body::set_direction(const Vec2f& dir) {
	direction_ = dir;
}

void Body::set_layer(Layer layer) {
	layer_ = layer;
}

void Body::set_velocity(float vel) {
	velocity_ = vel;
}

void Body::set_acceleration(float acc) {
	acceleration_ = acc;
}

void Body::set_mass(float mass) {
	mass_ = mass;
}

void Body::set_active(bool active) {
	is_active_ = active;
}

Body::entity_ptr Body::get_owner() const {
	auto o = owner_.lock();
	return o;
}

Collider Body::get_collider() const {
	return collider_;
}

Vec2f Body::get_position() const {
	return collider_.get_position();
}

Vec2f Body::get_direction() const {
	return direction_;
}

Layer Body::get_layer() const {
	return layer_;
}

float Body::get_velocity() const {
	return velocity_;
}

float Body::get_acceleration() const {
	return acceleration_;
}

float Body::get_mass() const {
	return mass_;
}

bool Body::is_active() const {
	return is_active_;
}

} // end of namespace physics
