#include "game.h"

#include <SDL.h>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_time_(1000 / 60)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_time_(1000 / 60)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
}

Game::~Game()
{
    
}

void Game::run()
{
	if (graphics_->is_ok()) {
		return;
	}

	if (!active_scene_) {
		printf("Dont have scenes!");
	}

    previous_time_ = SDL_GetTicks();
    
    bool quit = 0;
    while (!quit)
    {
		input_.handle();

        int32_t start = SDL_GetTicks();
        elapsed_ = start - previous_time_;
        previous_time_ = start;
        lag_ += elapsed_;
        
        while (lag_ >= tick_time_) {
			active_scene_->update(*this);
            lag_ -= tick_time_;
        }

		graphics_->clear_frame();
		active_scene_->render(*this);
        graphics_->render_frame();
    }
}



void Game::set_active_scene(const std::string &name) { 
    auto found = scenes_.find(name);
    if (found != scenes_.end()) {
        active_scene_ = found->second.get();
    }
}


void Game::add_scene(Scene::ptr scene, const std::string& name) {
    scenes_.emplace(name, std::move(scene));
	active_scene_ = scenes_.at(name).get();
}


Graphics &Game::get_graphics() { 
    return *graphics_;
}


Input &Game::get_input() { 
	return input_;
}
