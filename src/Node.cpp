//
//  Node.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 11/03/2023.
//

#include "Node.hpp"
#include "game.h"

#include <algorithm>
#include <iterator>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

NodeBase::~NodeBase() { 
	
}

void NodeBase::init(Game&) {

}

void NodeBase::update(Game&) {

}

void NodeBase::render(Game&) {

}

void NodeBase::deinit(Game&) {

}

glm::vec2 NodeBase::get_position() const {
	return pos;
}

glm::vec2 NodeBase::get_direction() const {
	return dir;
}

void NodeBase::set_position(glm::vec2 pos) {
	this->pos = pos;

	if (body)
	{
		body->set_position(pos);
	}
}

void NodeBase::set_direction(glm::vec2 dir) {
	this->dir = glm::normalize(dir);

	if (body)
	{
		body->set_direction(dir);
	}
}

void NodeBase::translate(float num) {
	prev_pos = pos;
	pos = pos + (dir * num);

	if (body)
	{
		body->set_position(pos);
	}
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

	std::copy(added_childrens.begin(), added_childrens.end(), std::back_inserter(childrens));
	added_childrens.clear();

	for (auto& c : childrens) {
		c->init_internal(game);
	}
}

void NodeBase::update_internal(Game& game) {
	prev_pos = pos;
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

	std::for_each(added_childrens.begin(), added_childrens.end(), [&game](NodePtr& n) {
		n->init_internal(game);
	});

	std::copy(added_childrens.begin(), added_childrens.end(), std::back_inserter(childrens));
	added_childrens.clear();
}

void NodeBase::add_node(const NodePtr &node) {
	added_childrens.emplace_back(std::move(node));
	added_childrens.back()->parent = this;
}

int NodeBase::num_childs() const
{
	return childrens.size();
}

NodePtr NodeBase::get_children(int index)
{
	return childrens[index];
}

Body* NodeBase::get_body() const
{
	return body;
}

void NodeBase::set_body(Body* body)
{
	this->body = body;
	pos = body->get_position();
	dir = body->get_direction();
}

void NodeBase::deinit_internal(Game& game)
{
	for (auto& n : childrens) {
		n->deinit_internal(game);
	}

	childrens.erase(std::remove_if(childrens.begin(), childrens.end(), [](auto& n) {
		return n->is_deleted();
	}), childrens.end());
	
	if (is_deleted() && body)
	{
		game.get_physics().remove(body);
	}
}


void NodeBase::sync_physic()
{
	if (body)
	{
		pos = body->get_position();
		dir = body->get_direction();
	}
	for (auto& c : childrens) {
		c->sync_physic();
	}
}
