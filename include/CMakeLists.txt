
set(HEADER_FILES 
    input.h
    graphics.h
    texture.h
    physics.h
    AnimationSprite.hpp
    game.h
    fonts.h
    Sprite.h
    Sprite_atlas.hpp
    log.h
    Audio.hpp
)

target_sources(${PROJECT_NAME} PUBLIC ${HEADER_FILES})