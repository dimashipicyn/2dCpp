
set(SOURCE_FILES 
    input.cpp
    graphics.cpp
    texture.cpp
    physics.cpp
    game.cpp
    Sprite.cpp
    AnimationSprite.cpp
    Sprite_atlas.cpp
    log.cpp
    Audio.cpp
    Node.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})