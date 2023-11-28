cmake_minimum_required(VERSION 3.20)
project(2dCpp VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(third_party)

add_library(${PROJECT_NAME})

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
	#SDL3_image::SDL3_image
    #SDL3_ttf::SDL3_ttf
	#SDL3_mixer::SDL3_mixer
    jsoncpp_lib
    glm
    png_shared
)

target_include_directories(${PROJECT_NAME} PRIVATE
	include
    third_party/SDL/include
    #third_party/SDL_image/include
    #third_party/SDL_mixer/include
    #third_party/SDL_ttf/include
	third_party/glm
    third_party/jsoncpp/include
    third_party/miniaudio 
)