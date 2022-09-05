#include "game.h"

#include <SDL.h>

Game::Game()
    : width_(800)
    , heigth_(600)
    , name_("default")
    , tick_time_(1000 / 60)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Could not initialize video or audio.\n");
        return;
    }
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
}

Game::Game(const Config &config)
    : width_(config.width)
    , heigth_(config.heigth)
    , name_(config.name)
    , tick_time_(1000 / 60)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Could not initialize video or audio.\n");
        return;
    }
    graphics_ = std::make_unique<Graphics>(width_, heigth_, name_);
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
        
        graphics_->clear_frame();

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
        
        graphics_->render_frame();
    }
}



void Game::set_active_scene(const std::string &name) { 
    auto found = scenes_.find(name);
    if (found != scenes_.end()) {
        active_scene_ = found->second.get();
    }
}


void Game::add_scene(Scene::ptr scene, const std::string& name) {
    scenes_.emplace(name, std::move(scene));
}


Graphics &Game::get_graphics() { 
    return *graphics_;
}
