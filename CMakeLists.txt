set(VCPKG_ROOT_DIR "$ENV{VCPKG_ROOT}" CACHE PATH "VCPKG_ROOT_DIR")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.0.0)
project(2dCpp)

add_compile_options(-Wall -Wextra -pedantic)

if (WIN32)
    set(VCPKG_SYSARCH "x64-windows")
elseif(APPLE)
    set(VCPKG_SYSARCH "x64-osx")
else()
    set(VCPKG_SYSARCH "x64-linux")
endif()

set(VCPKG_PREFIX_PATH "${VCPKG_ROOT_DIR}/installed/${VCPKG_SYSARCH}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VCPKG_PREFIX_CONF_PATH "${VCPKG_PREFIX_PATH}/debug")
else()
    set(VCPKG_PREFIX_CONF_PATH "${VCPKG_PREFIX_PATH}")
endif()

set(SOURCE_FILES
    src/input.cpp
    src/graphics.cpp
    src/texture.cpp
    #src/physics.cpp
    src/scene.cpp
    src/game.cpp
    src/Sprite.cpp
    src/AnimationSprite.cpp
    src/Sprite_atlas.cpp
    src/log.cpp
	src/Audio.cpp
    src/Node.cpp
)

set(HEADER_FILES 
    include/input.h
    include/graphics.h
    include/texture.h
    #include/physics.h
    include/AnimationSprite.hpp
    include/game.h
    include/scene.h
	include/fonts.h
    include/Sprite.h
    include/Sprite_atlas.hpp
    include/log.h
	include/Audio.hpp
    include/Node.hpp
)

set (CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

#add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


# find_library(APPKIT Appkit)
# if (NOT APPKIT)
#     message(FATAL_ERROR "Appkit not found")
# endif()

# find_library(OPENGL OpenGL)
# if (NOT OPENGL)
#     message(FATAL_ERROR "OpenGL not found")
# endif()

# add_subdirectory(utils)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # utils
    # "-framework Appkit"
    # "-framework OpenGL"
	SDL2::SDL2
	SDL2::SDL2main
	SDL2::SDL2-static
	SDL2::SDL2_image
	SDL2_ttf::SDL2_ttf-static
	SDL2_mixer::SDL2_mixer-static
    jsoncpp_object jsoncpp_static JsonCpp::JsonCpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${PROJECT_SOURCE_DIR}"
    # "${PROJECT_SOURCE_DIR}/utils"
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${VCPKG_PREFIX_PATH}/include
	include
)

export(TARGETS ${PROJECT_NAME} FILE 2dCppConfig.cmake)
