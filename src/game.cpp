#include "game.h"
#include "log.h"

#include <chrono>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_time_(1 / 60.0f)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_time_(1 / 60.0f)
{
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
	audio_ = std::make_unique<Audio>();
}

Game::~Game()
{
    
}

struct Chrono
{
	Chrono() {
		last_time = std::chrono::steady_clock::now();
	}

	float reset() {
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = now - last_time;
		last_time = now;
		return diff.count();
	}

	std::chrono::time_point<std::chrono::steady_clock> last_time;
};

void Game::run()
{
	if (!graphics_->is_ok()) {
		return;
	}

	if (!active_scene_) {
		LOG_ERROR("Dont have scenes!");
        return;
	}

    active_scene_->start_internal(*this);

	Chrono chr;

	previous_time_ = chr.reset();
    
    bool quit = false;
    while (!quit)
    {
		input_.handle();
        
        quit = input_.get_button(Input::Quit) != 0;

		elapsed_ = chr.reset();
        lag_ += elapsed_;
        
		while (lag_ >= (tick_time_ / 2)) {
			active_scene_->update_internal(*this);
            lag_ -= tick_time_;
        }

		graphics_->clear_frame();
		active_scene_->render_internal(*this);
        graphics_->render_frame();
    }
}

void Game::set_active_scene(const std::string &name) { 
    auto found = scenes_.find(name);
    if (found != scenes_.end()) {
        active_scene_ = found->second.get();
    }
    //active_scene_->start(*this);
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
	return tick_time_;
}


float Game::get_lag() const {
	return lag_;
}


Audio &Game::get_audio() {
	return *audio_;
}
