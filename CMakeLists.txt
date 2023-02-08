cmake_minimum_required(VERSION 3.22)
project(scop)

set(CMAKE_CXX_STANDARD 17)

SET(LIBS glfw dl pthread)

INCLUDE_DIRECTORIES(/System/Library/Frameworks)

if (APPLE)
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    FIND_LIBRARY(APPKIT_LIBRARY AppKit)
    SET(LIBS vulkan ${LIBS} ${OPENGL_LIBRARY} ${APPKIT_LIBRARY})
else ()
    SET(LIBS ${LIBS} vulkan X11 Xxf86vm Xrandr Xi)
endif ()

include_directories(include libs/glfw_osx/include ~/.brew/include)
include_directories(/Applications/VulkanSDK/1.2.189.0/MoltenVK/include)
link_directories(/Applications/VulkanSDK/1.2.189.0/macOS/lib ~/.brew/Lib)


add_executable(
        scop
        include/VkEngine.hpp
        source/main.cpp
        source/VkEngine.cpp
        include/utils.hpp
        source/utils.cpp
)

TARGET_LINK_LIBRARIES(scop ${LIBS})