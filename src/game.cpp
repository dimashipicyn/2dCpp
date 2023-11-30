#include "game.h"
#include "log.h"
#include "Common.hpp"
#include "Audio.hpp"

#include <Node.hpp>
#include <cassert>
#include <glm/common.hpp>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_time_(1 / 60.0f)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
	audio_ = std::make_unique<Audio>();
	physics_ = std::make_unique<Physics>(1.0f);
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_time_(1 / 60.0f)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
	audio_ = std::make_unique<Audio>();
	physics_ = std::make_unique<Physics>(1.0f);
}

Game::~Game()
{
    
}

void Game::run()
{
	if (!graphics_->is_ok()) {
		return;
	}

	if (scenes_.empty()) {
		LOG_ERROR("Dont have scenes!");
        return;
    }

	Chrono chr;
	elapsed_ = chr.reset();
    
    while (!quit_)
    {
        graphics_->clear_frame();

		input_.handle();
        quit_ = input_.get_button(Input::Quit) != 0;

        lag_ += elapsed_;

        if (scenes_.empty()) {
            LOG_ERROR("Dont have scenes!");
            return;
        }

        NodePtr current_scene = scenes_.top();
        current_scene->update_internal(*this);

        while (lag_ >= tick_time_) {
            physics_->step(tick_time_);
            current_scene->sync_physic();

            lag_ -= tick_time_;
        }

		current_scene->render_internal(*this);

        graphics_->render_frame();

        current_scene->deinit_internal(*this);

		elapsed_ = chr.reset();
    }
}

void Game::stop()
{
    quit_ = true;
}

void Game::push(NodePtr scene)
{
    scene->init_internal(*this);
    scenes_.push(scene);
}

void Game::pop()
{
    scenes_.top()->deinit_internal(*this);
    scenes_.pop();
}

Graphics &Game::get_graphics() { 
    return *graphics_;
}

Input &Game::get_input() { 
	return input_;
}

Audio &Game::get_audio() {
	return *audio_;
}

Physics &Game::get_physics() {
	return *physics_;
}

float Game::get_elapsed() const {
	return elapsed_;
}

float Game::get_tick() const {
	return tick_time_;
}


float Game::get_lag() const {
	return lag_;
}

void Game::set_fps(float fps) {
	fps = glm::clamp(fps, 30.0f, 120.0f);
}
