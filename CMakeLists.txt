cmake_minimum_required(VERSION 4.1)
project(TestGame)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.2)
FETCHCONTENT_MAKEAVAILABLE(SFML)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 20)

add_executable(TestGame main.cpp
        Vector/Vector.h
        Vector/Vector.cpp
        Includes/Includes.h
        WindowLayer/window.h
        WindowLayer/window.cpp
        WindowLayer/renderer.h
        WindowLayer/renderer.cpp
        Entities/Entities.h
        Entities/Entities.cpp
        Entities/steering.h
        Entities/steering.cpp
        Init/Init.h
        Init/Init.cpp
)

target_link_libraries(TestGame sfml-graphics)