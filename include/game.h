#ifndef GAME_H
# define GAME_H

#include "graphics.h"
#include "input.h"
#include "Audio.hpp"
#include "physics.h"

#include <Node.hpp>
#include <cstdint>
#include <string>
#include <stack>

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
	Physics& get_physics();

	float get_tick() const;
	float get_elapsed() const;
	float get_lag() const;

	void set_fps(float fps);

    void push(const NodePtr& scene);
    void pop();
    
    void run();
    
private:
	
    int32_t         width_ = 0;
    int32_t         heigth_ = 0;
    std::string     name_;
    
    float         	tick_time_ = 0.0f;
	float			lag_ = 0.0f;
	float			elapsed_ = 0.0f;
	float			fps = 60.0f;

	Input input_;
    std::unique_ptr<Graphics> graphics_;
	std::unique_ptr<Audio> audio_;
	std::unique_ptr<Physics> physics_;
    std::stack<NodePtr> scenes_;
};


#endif
