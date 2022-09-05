#include "scene.h"
#include "entity.h"

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::preload(Game& game)
{
}

void Scene::create(Game& game)
{
}

void Scene::update(Game& game)
{
    for (auto& entity : entities_) {
        if (entity.first) {
            entity.second->input(game);
            entity.second->update(game);
        }
    }

    for (auto entity_itr = entities_.begin(); entity_itr != entities_.end();) {
        if (!entity_itr->first) {
            std::swap(*entity_itr, entities_.back());
            entities_.pop_back();
        }
        else {
            entity_itr++;
        }
    }
    
    entities_.insert(entities_.end(), std::make_move_iterator(added_entities_.begin()), std::make_move_iterator(added_entities_.end()));
}

void Scene::render(Game& game)
{
    for (auto& entity : entities_) {
        entity.second->render(game);
    }
}

void Scene::attach_entity(Entity::ptr entity)
{
    entities_.emplace_back(true, std::move(entity));
}

void Scene::detach_entity(Entity::ptr entity)
{
    auto found = std::find_if(entities_.begin(), entities_.end(), [&entity](auto& e){return e.second == entity;});
    if (found != entities_.end()) {
        found->first = false;
    }
}
