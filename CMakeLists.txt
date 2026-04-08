cmake_minimum_required(VERSION 3.10)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SnakeGame
    main.cpp
    Game.cpp
    Snake.cpp
    Food.cpp
    Renderer.cpp
    Input.cpp
    SaveSystem.cpp
)

if (WIN32)
    target_compile_definitions(SnakeGame PRIVATE _WIN32_WINNT=0x0601)
endif()
