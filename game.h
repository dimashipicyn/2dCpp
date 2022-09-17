#ifndef GAME_H
# define GAME_H

#include "graphics.h"
#include "scene.h"
#include "input.h"

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

	int32_t get_tick() const;
	int32_t get_elapsed() const;
    
    void add_scene(Scene::ptr scene, const std::string& name);
    void set_active_scene(const std::string& name);
    
    void run();
    
private:
	
    int32_t         width_;
    int32_t         heigth_;
    std::string     name_;
    
    int32_t         tick_time_;
	int32_t			lag_;
	int32_t			previous_time_;
    int32_t     	elapsed_;
    
    std::map<std::string, Scene::ptr> scenes_;
    Scene* active_scene_;

	Input input_;
    std::unique_ptr<Graphics> graphics_;
};


#endif
