//
//  entity.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 05/09/2022.
//

#include "entity.h"

Entity::Entity()
    : position_()
    , direction_()
{
}

Entity::~Entity() {
}

void Entity::rotate(float rot) {
    direction_ = direction_.rotate(rot);
}

void Entity::translate(float tr) {
    auto v = direction_.scalar(tr);
    position_ = position_.add(v);
}

Vec2f Entity::get_position() const {
    return position_;
}
Vec2f Entity::get_direction() const {
    return direction_;
}

void Entity::set_direction(const Vec2f &dir) { 
    direction_ = dir;
	//direction_ = direction_.normalize();
}


void Entity::set_position(const Vec2f &pos) {
    position_ = pos;
}
