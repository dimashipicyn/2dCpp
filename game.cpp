#include "stdafx.h"

#include "game.h"
#include "log.h"
#include "Chronometer.h"

#include <SDL.h>
#include <thread>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_(1000 / 60.0)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_(1000 / 60.0)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
	audio_ = std::make_unique<Audio>();
}

Game::~Game()
{
    
}

void Game::run()
{
	if (!graphics_->is_ok()) {
		return;
	}

	if (!active_scene_) {
		LOG_ERROR("Dont have scenes!");
        assert(false);
	}

    float frame_time = 1000 / 240.0;

    Chronometer chronometer;
    elapsed_ = tick_;
    
    bool quit = false;
    while (!quit)
    {
        lag_ += elapsed_;

		input_.handle();
        
        quit = input_.get_button(Input::Quit) != 0;
        
        while (lag_ >= tick_) {
			active_scene_->update(*this);
            lag_ -= tick_;
        }

		graphics_->clear_frame();
		active_scene_->render(*this);
        graphics_->render_frame();

        elapsed_ = chronometer.delta_ms();
        
        float frame_until = frame_time - elapsed_;

        chronometer.sleep_until(frame_until);
        elapsed_ += chronometer.delta_ms();
    }
}

void Game::set_active_scene(const std::string &name) { 
    auto found = scenes_.find(name);
    if (found != scenes_.end()) {
        active_scene_ = found->second.get();
    }
    active_scene_->start(*this);
}


void Game::add_scene(Scene::ptr scene, const std::string& name) {
    scenes_.emplace(name, std::move(scene));
}

Graphics &Game::get_graphics() { 
    return *graphics_;
}

Input &Game::get_input() { 
	return input_;
}

float Game::get_elapsed() const { 
	return elapsed_;
}

float Game::get_tick() const { 
	return tick_;
}


float Game::get_lag() const { 
	return lag_;
}

Audio &Game::get_audio() {
	return *audio_;
}

Scene* Game::get_active_scene() {
	return active_scene_;
}
