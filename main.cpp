#include "game.h"
#include "simple_scene.hpp"

int main(int ac, char** argv, char** env) {
	(void)ac;
	(void)argv;
    (void)env;

	Config config;
	config.width = 640;
	config.heigth = 480;
	config.name = "Breakout";

	Game game(config);
    
    game.add_scene(std::make_unique<Simple_scene>(), "simple scene");
    game.set_active_scene("simple scene");
    game.run();

    return 0;
}
