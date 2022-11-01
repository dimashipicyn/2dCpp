#include "physics.h"
#include "entity.h"

#include <cstdlib>
#include <SDL.h>
#include <cassert>
#include <cmath>

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
	const auto& aabb_one = one.get_collider().get_aabb();
	const auto& aabb_two = two.get_collider().get_aabb();

	Vec2f one_half_size(aabb_one.size.x / 2, aabb_one.size.y / 2);
	Vec2f two_half_size(aabb_two.size.x / 2, aabb_two.size.y / 2);

	Vec2f center_one(aabb_one.pos.x + one_half_size.x, aabb_one.pos.y + one_half_size.y);
	Vec2f center_two(aabb_two.pos.x + two_half_size.x, aabb_two.pos.y + two_half_size.y);
	Vec2f diff = center_one.sub(center_two);

	Vec2f normal(0, 0);
	float penetration = 0;

	float x_overlap = (one_half_size.x + two_half_size.x) - fabs(diff.x);
	if (x_overlap > 0) {
		float y_overlap = (one_half_size.y + two_half_size.y) - fabs(diff.y);
		if (x_overlap < y_overlap) {
			if (diff.x < 0) {
				normal = Vec2f(-1, 0);
			}
			else {
				normal = Vec2f(1, 0);
			}
			penetration = x_overlap;
		}
		else {
			if (diff.y < 0) {
				normal = Vec2f(0, -1);
			}
			else {
				normal = Vec2f(0, 1);
			}
			penetration = y_overlap;
		}
	}

	Vec2f one_dir = one.get_direction();
	float one_vel = one.get_velocity();
	Vec2f two_dir = two.get_direction();
	float two_vel = two.get_velocity();
	one.set_direction(one_dir.add(two_dir.scalar(two_vel)).normalize());
	one.set_velocity(one_vel + (two_vel * one_dir.scalar(two_dir)));

	one.set_position(one.get_position().add(normal.scalar(penetration + 1)));
	one_dir = one.get_direction();
	if (normal.x) {
		one.set_direction(Vec2f(-one_dir.x, one_dir.y));
	}
	else {
		one.set_direction(Vec2f(one_dir.x, -one_dir.y));
	}

	one.has_collision_ = true;
	one.collisions_.push_back(two.get_owner());

	two.has_collision_ = true;
	two.collisions_.push_back(one.get_owner());
}

void World::process_collision(Body& one, Body& two) {
	int layer_one = static_cast<int>(one.get_layer());
	int layer_two = static_cast<int>(two.get_layer());

	if (!(layer_one & layer_two)) {
		return;
	}

	bool has_intersect = one.get_collider().has_intersection(two.get_collider());
	if (has_intersect) {
		resolve_collision(one, two);
	}
}

void World::step(float dt)
{
	for (auto& two : static_body_)
	{
		two->has_collision_ = false;
		two->collisions_.clear();
	}

    for (auto& one : dynamic_body_)
    {
        if (!one->is_active()) {
            continue;
        }
		one->has_collision_ = false;
		one->collisions_.clear();

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
//			bool is_already_collision = false;
//			for (auto& two_col : two->collisions_) {
//				if (auto p = two_col.lock(); p && p->get_body().get() == one.get()) {
//					is_already_collision = true;
//				}
//			}
//			if (is_already_collision) {
//				continue;
//			}
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

const AABB& Collider::get_aabb() const {
	assert(type_ == ColliderType::AABB);
	return aabb_;
}

const Circle& Collider::get_circle() const {
	assert(type_ == ColliderType::CIRCLE);
	return circle_;
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
	, has_collision_(false)
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

Body::vector_collisions Body::get_collisions() const {
	return collisions_;
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
