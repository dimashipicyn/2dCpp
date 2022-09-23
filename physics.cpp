#include "physics.h"

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

//bool Physic::has_intersection(const AABB &aabb) {
//
//    for (auto& body : dynamic_body_)
//    {
//        if (body->aabb.has_intersection(aabb)) {
//            return true;
//        }
//    }
//    return false;
//}

void World::resolve_collision(Body& one, Body& two)
{
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

void World::step()
{
    for (auto& one : dynamic_body_)
    {
        if (!one->is_active()) {
            continue;
        }

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

} // end of namespace physics
