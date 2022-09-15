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
    main.cpp
    input.cpp
    graphics.cpp
    texture.cpp
    entity.cpp
    physics.cpp
    vector.cpp
    scene.cpp
    game.cpp
    Sprite.cpp
    simple_scene.cpp
    Sprite_atlas.cpp
)

set(HEADER_FILES 
    input.h
    graphics.h
    texture.h
    entity.h
    physics.h
    vector.h
    animation.h
    game.h
    scene.h
	fonts.h
    Sprite.h
    Sprite_atlas.hpp
)

set (CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)


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
)
