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
        if (entity.is_attached) {
            entity.ptr->input(game);
            entity.ptr->update(game);
        }
    }

    for (auto entity_itr = entities_.begin(); entity_itr != entities_.end();) {
        if (!entity_itr->is_attached) {
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
        entity.ptr->render(game);
    }
}

void Scene::attach_entity(Entity::ptr entity)
{
    entities_.emplace_back(true, entity);
}

void Scene::detach_entity(Entity::ptr entity)
{
    auto found = std::find_if(entities_.begin(), entities_.end(), [&entity](auto& e){return e.ptr == entity;});
    if (found != entities_.end()) {
        found->is_attached = false;
    }
}
