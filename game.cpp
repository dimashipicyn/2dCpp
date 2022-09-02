#include "game.h"

#include <chrono>
#include <SDL.h>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_time_(1000 / 60)
{
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_time_(1000 / 60)
{
}

Game::~Game()
{
    
}

void Game::run()
{
    previous_time_ = SDL_GetTicks();
    
    SDL_Event event;
    bool quit = 0;
    while (!quit)
    {
        SDL_PollEvent(&event);
        
//        game->mouse.is_press_l = 0;
//        game->mouse.is_press_r = 0;
        switch (event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;
//            case SDL_MOUSEMOTION:
//                SDL_GetMouseState(&game->mouse.x, &game->mouse.y);
//                break;
//            case SDL_MOUSEBUTTONUP:
//                game->mouse.is_press_l = 1;
//                break;
        }
        
        //game->keys = SDL_GetKeyboardState(NULL);
        
//        clear_frame(game->graphics);
//
//        loop_callback(game);
//
//        render_frame(game->graphics);
        int32_t start = SDL_GetTicks();
        elapsed_ = start - previous_time_;
        previous_time_ = start;
        lag_ += elapsed_;
        
        while (lag_ >= tick_time_) {
            //step_physic_world();
            //scene_update(game_ctx->active_scene, game_ctx);
            lag_ -= tick_time_;
        }
        
        //scene_render(game_ctx->active_scene, game_ctx);
    }
}

