//
//  Node.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 11/03/2023.
//

#ifndef Node_hpp
#define Node_hpp

#include "Export.h"
#include "Common.hpp"
#include "physics.h"

#include <vector>
#include <glm/vec2.hpp>

class Game;
class NodeBase;

using NodePtr = Ptr<NodeBase>;

class TWODCPP_EXPORT NodeBase
{
public:
    NodeBase() = default;
	virtual ~NodeBase();
	virtual void init(Game& game);
	virtual void update(Game& game);
	virtual void render(Game& game);
	virtual void deinit(Game& game);

	glm::vec2 get_position() const;
	glm::vec2 get_direction() const;

	void set_position(glm::vec2 pos);
	void set_direction(glm::vec2 dir);

	void translate(float num);
	void rotate(float angle);

	void add_node(const NodePtr& node);

	bool is_deleted() const;
	void delete_node();

	int num_childs() const;
	NodePtr get_children(int index);

	Body* get_body() const;
	void set_body(Body* body);

protected:
	glm::vec2 prev_pos;

private:
	friend class Scene;
	friend class Game;

	void init_internal(Game& game);
	void update_internal(Game& game);
	void render_internal(Game& game);
	void deinit_internal(Game& game);
	void sync_physic();

private:
	glm::vec2 pos;
	glm::vec2 dir;

	Body* body = nullptr;
	NodeBase* parent = nullptr;
	
	std::vector<NodePtr> childrens;
	std::vector<NodePtr> added_childrens;
	
	bool deleted = false;
};

#endif /* Node_hpp */
