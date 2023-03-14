#include "scene.h"

Scene::Scene() {
}

Scene::~Scene() noexcept {
}

void Scene::start(Game& /*game*/)
{
}

void Scene::update(Game& game)
{

}

void Scene::render(Game& game)
{
	for (NodePtr& n : nodes) {
		n->render(game);
	}
}

void Scene::stop(Game& game)
{

}

void Scene::start_internal(Game& game)
{
	start(game);
	for (auto& n : nodes) {
		n->init_internal(game);
	}

	process_nodes(game);
}

void Scene::update_internal(Game& game)
{
	update(game);
	for (auto& n : nodes) {
		n->update_internal(game);
	}
}

void Scene::render_internal(Game& game)
{
	render(game);
	for (auto& n : nodes) {
		n->render_internal(game);
	}

	process_nodes(game);
}

void Scene::stop_internal(Game& game)
{
	stop(game);
}

void Scene::add_node(NodePtr node) { 
	added_nodes.emplace_back(std::move(node));
}

void Scene::process_nodes(Game& game)
{
	nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [](const NodePtr& n) {
		return n->is_deleted();
	}), nodes.end());

	std::for_each(added_nodes.begin(), added_nodes.end(), [&game](NodePtr& n) {
		n->init(game);
	});

	std::copy(added_nodes.begin(), added_nodes.end(), std::back_inserter(nodes));
	added_nodes.clear();
}

