//
//  Node.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 11/03/2023.
//

#include "Node.hpp"

#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

NodeBase::~NodeBase() { 
	
}

void NodeBase::init(Game& game) {

}

void NodeBase::update(Game& game) {

}

void NodeBase::render(Game& game) {

}

glm::vec2 NodeBase::get_position() const {
	return pos;
}

glm::vec2 NodeBase::get_direction() const {
	return dir;
}

void NodeBase::set_position(glm::vec2 pos) {
	this->pos = pos;
}

void NodeBase::set_direction(glm::vec2 dir) {
	this->dir = glm::normalize(dir);
}

void NodeBase::translate(float num) {
	pos = pos + (dir * num);
}

void NodeBase::rotate(float angle) {
	dir = glm::rotate(dir, angle);
}

bool NodeBase::is_deleted() const { 
	return deleted;
}

void NodeBase::delete_node() { 
	deleted = true;
}

void NodeBase::init_internal(Game &game) {
	init(game);
	for (auto& c : childrens) {
		c->init_internal(game);
	}
}

void NodeBase::update_internal(Game& game) {
	update(game);
	for (auto& c : childrens) {
		c->update_internal(game);
	}

}

void NodeBase::render_internal(Game &game) {
	render(game);
	for (auto& c : childrens) {
		c->render_internal(game);
	}
}

void NodeBase::add_node(const NodePtr &node) {
	childrens.emplace_back(node);
	childrens.back()->parent = this;
}

