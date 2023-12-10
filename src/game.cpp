#include "game.h"

#include "graphics.h"
#include "input.h"
#include "Audio.hpp"
#include "physics.h"
#include "Node.hpp"
#include "Resources.h"
#include "Provider.h"
#include "log.h"
#include "Gui.h"

#include <cassert>
#include <glm/common.hpp>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_time_(1 / 60.0f)
{
    input_ = std::make_unique<Input>();
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
    audio_ = std::make_unique<Audio>();
    physics_ = std::make_unique<Physics>(1.0f);
    resources_ = std::make_unique<Resources>(*this);
    gui_ = std::make_unique<Gui>();
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_time_(1 / 60.0f)
{
    input_ = std::make_unique<Input>();
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
	audio_ = std::make_unique<Audio>();
	physics_ = std::make_unique<Physics>(1.0f);
    resources_ = std::make_unique<Resources>(*this);
    gui_ = std::make_unique<Gui>();

    Provider::get().provide<Game>(this);
    Provider::get().provide<Graphics>(graphics_.get());
    Provider::get().provide<Audio>(audio_.get());
    Provider::get().provide<Physics>(physics_.get());
    Provider::get().provide<Resources>(resources_.get());
    Provider::get().provide<Gui>(gui_.get());
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

		input_->handle();
        quit_ = input_->get_button(Input::Quit) != 0;

        lag_ += elapsed_;

        if (scenes_.empty()) {
            LOG_ERROR("Dont have scenes!");
            return;
        }

        NodePtr current_scene = scenes_.top();
        current_scene->update_internal(*this);
        
        gui_->update(*this);

        while (lag_ >= tick_time_) {
            physics_->step(tick_time_);
            current_scene->sync_physic();

            lag_ -= tick_time_;
        }

		current_scene->render_internal(*this);
        gui_->render(*this);

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
	return *input_;
}

Audio &Game::get_audio() {
	return *audio_;
}

Physics &Game::get_physics() {
	return *physics_;
}

Resources& Game::resources()
{
    return *resources_;
}

Gui& Game::gui()
{
    return *gui_;
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
