
set(SOURCE_FILES 
    AnimationSprite.cpp
    Audio.cpp
    Common.cpp
    Font.cpp
    game.cpp
    graphics.cpp
    input.cpp
    log.cpp
    Node.cpp
    physics.cpp
    #Sprite_atlas.cpp
    Sprite.cpp
    SpriteNode.cpp
    texture.cpp
    Provider.cpp
    Gui.cpp
    Widgets.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
