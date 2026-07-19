cmake_minimum_required(VERSION 3.25)
project(TestGame LANGUAGES CXX)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

add_executable(TestGame
    main.cpp
    WindowLayer/window.cpp
    WindowLayer/renderer.cpp
    Entities/Entities.cpp
    Entities/steering.cpp
    Entities/Spawn.cpp
    Init/Init.cpp
    Collisions/Collisions.cpp
    Game/GameLogic.cpp
)

target_link_libraries(TestGame PRIVATE SFML::Graphics)

target_compile_features(TestGame PRIVATE cxx_std_20)

set(GAME_ASSETS
    assets/player.png
    assets/bullet.png
    assets/bullet2.png
    assets/explosion.png
    assets/background.png
    assets/arial.ttf
)

foreach(ASSET IN LISTS GAME_ASSETS)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${ASSET}")
        add_custom_command(
            TARGET TestGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/${ASSET}"
                    "$<TARGET_FILE_DIR:TestGame>"
            VERBATIM
        )
    endif()
endforeach()

if(MINGW)
    target_link_options(TestGame PRIVATE -static-libgcc -static-libstdc++)

    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    if(EXISTS "${MINGW_BIN_DIR}/libwinpthread-1.dll")
        add_custom_command(
            TARGET TestGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN_DIR}/libwinpthread-1.dll"
                    "$<TARGET_FILE_DIR:TestGame>"
            VERBATIM
        )
    endif()
endif()
