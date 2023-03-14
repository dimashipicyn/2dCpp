//
//  Node.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 11/03/2023.
//

#ifndef Node_hpp
#define Node_hpp

#include <vector>
#include <glm/vec2.hpp>

class Game;
class NodeBase;

using NodePtr = std::shared_ptr<NodeBase>;

template<class T>
using Ptr = std::shared_ptr<T>;

template<class T>
auto CreatePtr = std::make_shared<T>;

class NodeBase
{
public:
	virtual ~NodeBase();
	virtual void init(Game& game);
	virtual void update(Game& game);
	virtual void render(Game& game);

	glm::vec2 get_position() const;
	glm::vec2 get_direction() const;

	void set_position(glm::vec2 pos);
	void set_direction(glm::vec2 dir);

	void translate(float num);
	void rotate(float angle);

	void add_node(const NodePtr& node);

	bool is_deleted() const;
	void delete_node();

private:
	friend class Scene;

	void init_internal(Game& game);
	void update_internal(Game& game);
	void render_internal(Game& game);

private:
	glm::vec2 pos;
	glm::vec2 dir;

	NodeBase* parent;
	std::vector<NodePtr> childrens;
	bool deleted = false;
};

#endif /* Node_hpp */
