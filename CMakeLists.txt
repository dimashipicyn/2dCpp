cmake_minimum_required(VERSION 3.20)
project(2dCpp VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(third_party)

add_library(${PROJECT_NAME})

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    #jsoncpp_lib
    glm
    png_shared
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
    third_party/SDL/include
	third_party/glm
    #third_party/jsoncpp/include
    third_party/miniaudio
    third_party/libpng
)