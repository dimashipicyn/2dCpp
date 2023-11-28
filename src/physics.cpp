#include "stdafx.h"

#include "physics.h"

#include <cstdlib>
#include <SDL.h>
#include <cassert>
#include <cmath>

#include <glm/geometric.hpp>

//namespace physics {

Physics::Physics(float gravity)
	: static_body_()
	, dynamic_body_()
	, gravity_(gravity)
{
}

Physics::~Physics() {
	static_body_.clear();
	dynamic_body_.clear();
}

void Physics::resolve_collision(Body& one, Body& two)
{
	AABB aabb_one = one.get_collider().get_aabb();
	AABB aabb_two = two.get_collider().get_aabb();

	glm::vec2 one_half_size(aabb_one.size.x / 2, aabb_one.size.y / 2);
	glm::vec2 two_half_size(aabb_two.size.x / 2, aabb_two.size.y / 2);

	glm::vec2 center_one(aabb_one.pos.x + one_half_size.x, aabb_one.pos.y + one_half_size.y);
	glm::vec2 center_two(aabb_two.pos.x + two_half_size.x, aabb_two.pos.y + two_half_size.y);
	glm::vec2 diff = center_one - center_two;

	glm::vec2 normal(0, 0);
	float penetration = 0;

	float x_overlap = (one_half_size.x + two_half_size.x) - fabs(diff.x);
	if (x_overlap > 0) {
		float y_overlap = (one_half_size.y + two_half_size.y) - fabs(diff.y);
		if (x_overlap < y_overlap) {
			if (diff.x < 0) {
				normal = glm::vec2(-1, 0);
			}
			else {
				normal = glm::vec2(1, 0);
			}
			penetration = x_overlap;
		}
		else {
			if (diff.y < 0) {
				normal = glm::vec2(0, -1);
			}
			else {
				normal = glm::vec2(0, 1);
			}
			penetration = y_overlap;
		}
	}

	glm::vec2 one_dir = one.get_direction();
	float one_vel = one.get_velocity();
	glm::vec2 two_dir = two.get_direction();
	float two_vel = two.get_velocity();
	one.set_direction(glm::normalize(one_dir + (two_dir * two_vel)));
	one.set_velocity(one_vel + (two_vel * glm::dot(one_dir, two_dir)));

	one.set_position(one.get_position() + normal * (penetration + 1));
	one_dir = one.get_direction();
	if (normal.x) {
		one.set_direction(glm::vec2(-one_dir.x, one_dir.y));
	}
	else {
		one.set_direction(glm::vec2(one_dir.x, -one_dir.y));
	}

	if (one.callback_)
	{
		one.callback_(one, two);
	}
	if (two.callback_)
	{
		two.callback_(two, one);
	}
}

void Physics::process_collision(Body& one, Body& two) {
	int bit_one = static_cast<int>(one.get_bit_mask());
	int bit_two = static_cast<int>(two.get_bit_mask());

	if (!(bit_one & bit_two)) {
		return;
	}

	bool has_intersect = one.get_collider().has_intersection(two.get_collider());
	if (has_intersect) {
		resolve_collision(one, two);
	}
}

void Physics::step(float dt)
{
    for (auto& one : dynamic_body_)
    {
        if (!one->is_enabled()) {
            continue;
        }

        for (auto& two : static_body_)
        {
            if (!two->is_enabled()) {
                continue;
            }
			process_collision(*one, *two);
        }

        for (auto& two : dynamic_body_)
        {
            if (!two->is_enabled() || (one.get() == two.get())) {
                continue;
            }

			process_collision(*one, *two);
        }
    }
}

Body* Physics::create(const AABB& aabb, bool dynamic)
{
	BodyPtr body = CreatePtr<Body>(aabb);
	if (dynamic) {
		dynamic_body_.push_back(body);
	}
	else {
		static_body_.push_back(body);
	}
	return body.get();
}

Body* Physics::create(const Circle& circle, bool dynamic)
{
	BodyPtr body = CreatePtr<Body>(circle);
	if (dynamic) {
		dynamic_body_.push_back(body);
	}
	else {
		static_body_.push_back(body);
	}
	return body.get();
}

void Physics::remove(const Body* body)
{
	auto predicat = [body](const auto& other)
	{
		return body == other.get();
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
	if ((circle_1.radius + circle_2.radius) <= glm::length(circle_1.pos - circle_2.pos)) {
		return true;
	}
	return false;
}

bool Collider::has_intersection(const AABB& aabb, const Circle& circle) const {
	
	return false;
}

glm::vec2 Collider::get_position() const {
	if (type_ == ColliderType::CIRCLE) {
		return circle_.pos;
	}
	else {
		return aabb_.pos;
	}
}

void Collider::set_position(const glm::vec2& pos) {
	if (type_ == ColliderType::CIRCLE) {
		circle_.pos = pos;
	}
	else {
		aabb_.pos = pos;
	}
}

const AABB& Collider::get_aabb() const {
	assert(type_ == ColliderType::AABB);
	return aabb_;
}

const Circle& Collider::get_circle() const {
	assert(type_ == ColliderType::CIRCLE);
	return circle_;
}

Body::Body()
	: collider_(AABB())
	, position_()
	, direction_()
	, bitmask_(BIT_0)
	, velocity_(0.0f)
	, mass_(0.0f)
	, is_enabled_(true)
{

}

Body::Body(const AABB& aabb)
	: collider_(aabb)
	, position_()
	, direction_()
	, bitmask_(BIT_0)
	, velocity_(0)
	, mass_(0)
	, is_enabled_(true)
{

}

Body::Body(const Circle& circle)
	: collider_(circle)
	, position_()
	, direction_()
	, bitmask_(BIT_0)
	, velocity_(0)
	, mass_(0)
	, is_enabled_(true)
{

}

Body::~Body()
{

}

void Body::on_collision(OnCollisionCallback callback) {
	callback_ = callback;
}

void Body::set_position(const glm::vec2& pos) {
	collider_.set_position(pos);
}

void Body::set_direction(const glm::vec2& dir) {
	direction_ = dir;
}

void Body::set_bit_mask(BitMask bit) {
	bitmask_ = bit;
}

void Body::set_velocity(float vel) {
	velocity_ = vel;
}

void Body::set_mass(float mass) {
	mass_ = mass;
}

void Body::set_enable(bool enable) {
	is_enabled_ = enable;
}

Collider Body::get_collider() const {
	return collider_;
}

glm::vec2 Body::get_position() const {
	return collider_.get_position();
}

glm::vec2 Body::get_direction() const {
	return direction_;
}

BitMask Body::get_bit_mask() const {
	return bitmask_;
}

float Body::get_velocity() const {
	return velocity_;
}

float Body::get_mass() const {
	return mass_;
}

bool Body::is_enabled() const {
	return is_enabled_;
}

//} // end of namespace physics
