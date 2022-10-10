//
//  entity.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 05/09/2022.
//

#include "entity.h"

Entity::Entity()
    : body_(std::make_shared<physics::Body>())
{
}

Entity::~Entity() {
}

void Entity::rotate(float rot) {
	body_->set_direction(body_->get_direction().rotate(rot));
}

void Entity::translate(float tr) {
	auto v = body_->get_direction().scalar(tr);
	body_->set_position(body_->get_position().add(v));
}

Vec2f Entity::get_position() const {
	return body_->get_position();
}
Vec2f Entity::get_direction() const {
	return body_->get_direction();
}

Entity::body_ptr Entity::get_body() {
	return body_;
}

void Entity::set_direction(const Vec2f &dir) { 
    body_->set_direction(dir);
}

void Entity::set_position(const Vec2f &pos) {
	body_->set_position(pos);
}

void Entity::set_body(body_ptr body) {
	body_ = std::move(body);
}
