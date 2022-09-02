#ifndef GAME_H
# define GAME_H

#include "graphics.h"
#include "keys.h"
#include "scene.h"

#include <cstdint>
#include <string>

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
    
    void run();
    
private:

//    t_graphics      *graphics;
//    t_scene         *scenes[TOTAL_SCENES];
//    t_scene         *active_scene;
//    const uint8_t    *keys;
//    t_mouse            mouse;
    int32_t         width_;
    int32_t         heigth_;
    std::string     name_;
    
    int32_t         tick_time_;
	int32_t			lag_;
	int32_t			previous_time_;
    int32_t     	elapsed_;
};


#endif
