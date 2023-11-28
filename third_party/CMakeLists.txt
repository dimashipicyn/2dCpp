
# https://github.com/libsdl-org/SDL/issues/6454
if(APPLE)
    enable_language(OBJC)
endif()

add_subdirectory(SDL)
add_subdirectory(glm)
#add_subdirectory(jsoncpp)
add_subdirectory(libpng)
