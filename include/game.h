#ifndef GAME_H
# define GAME_H

#include "Export.h"
#include "graphics.h"
#include "input.h"
#include "Audio.hpp"
#include "physics.h"
#include "Widgets.h"
#include "Font.h"
#include "Node.hpp"
#include "Resources.h"

#include <cstdint>
#include <memory>
#include <string>
#include <stack>
#include <vector>

struct TWODCPP_EXPORT Config
{
    int32_t width;
    int32_t heigth;
    std::string name;
};

class TWODCPP_EXPORT Game
{
public:
    Game();
    Game(const Config& config);
    ~Game();

    Graphics& get_graphics();
    Input& get_input();
    Audio& get_audio();
    Physics& get_physics();
    Resources& resources();

	float get_tick() const;
	float get_elapsed() const;
	float get_lag() const;

	void set_fps(float fps);

    void push(NodePtr scene);
    void pop();

    void run();
    void stop();
    
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
    std::unique_ptr<Resources> resources_;
    std::stack<NodePtr> scenes_;

    bool quit_ = false;
};


#endif
