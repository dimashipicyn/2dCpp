//
//  scene.h
//  solong
//
//  Created by Dmitry Shipicyn on 05.07.2022.
//

#ifndef scene_h
#define scene_h

#include "Node.hpp"

#include <vector>

class Game;

class Scene
{
public:
    using ptr = std::unique_ptr<Scene>;
    
    Scene();
    virtual ~Scene() noexcept;
    
    virtual void start(Game& game);
    virtual void update(Game& game);
    virtual void render(Game& game);
    virtual void stop(Game& game);

	void add_node(NodePtr node);

private:
    friend class Game;
    void start_internal(Game& game);
    void update_internal(Game& game);
    void render_internal(Game& game);
    void stop_internal(Game& game);

	void process_nodes(Game& game);

private:
	std::vector<NodePtr> nodes;
	std::vector<NodePtr> added_nodes;
};

#endif /* scene_h */
