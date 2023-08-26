
set(HEADER_FILES 
    AnimationSprite.hpp
    Audio.hpp
    Common.hpp
    Font.h
    game.h
    graphics.h
    input.h
    log.h
    Node.hpp
    physics.h
    Sprite_atlas.hpp
    Sprite.h
    SpriteNode.hpp
    texture.h
    Widgets.h
)

target_sources(${PROJECT_NAME} PUBLIC ${HEADER_FILES})