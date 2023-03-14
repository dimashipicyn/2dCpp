#ifndef GAME_H
# define GAME_H

#include "graphics.h"
#include "scene.h"
#include "input.h"
#include "Audio.hpp"

#include <cstdint>
#include <string>
#include <map>

struct Config
{
    int32_t width;
    int32_t heigth;
    std::string name;
};

class Game
{
public:
    Game();
    Game(const Config& config);
    ~Game();
    
    Graphics& get_graphics();
	Input& get_input();
	Audio& get_audio();

	float get_tick() const;
	float get_elapsed() const;
	float get_lag() const;
    
    void add_scene(Scene::ptr scene, const std::string& name);
    void set_active_scene(const std::string& name);
    
    void run();
    
private:
	
    int32_t         width_ = 0;
    int32_t         heigth_ = 0;
    std::string     name_;
    
    float         	tick_time_ = 0.0f;
	float			lag_ = 0.0f;
	float			previous_time_ = 0.0f;
    float     		elapsed_ = 0.0f;
    
    std::map<std::string, Scene::ptr> scenes_;
    Scene* active_scene_;

	Input input_;
    std::unique_ptr<Graphics> graphics_;
	std::unique_ptr<Audio> audio_;
};


#endif
