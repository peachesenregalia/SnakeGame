cmake_minimum_required(VERSION 3.10)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SnakeGame
    main.cpp
    core/GameLoop.cpp
    core/StateMachine.cpp
    systems/InputSystem.cpp
    systems/RenderSystem.cpp
    systems/TimeSystem.cpp
    gameplay/Snake.cpp
    gameplay/Food.cpp
    persistence/SaveSystem.cpp
)

target_include_directories(SnakeGame PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/systems
    ${CMAKE_SOURCE_DIR}/gameplay
    ${CMAKE_SOURCE_DIR}/persistence
)

if (WIN32)
    target_compile_definitions(SnakeGame PRIVATE _WIN32_WINNT=0x0601)
endif()
