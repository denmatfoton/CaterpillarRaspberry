cmake_minimum_required(VERSION 3.10)
project(caterpillar_remote_control)

find_package(Threads REQUIRED)
set(TARGET_NAME robot_control)

set(CMAKE_CXX_STANDARD 17)
set(COMMON_CXX_FLAGS
    -Wall
    -Wconversion
    -Werror
    -Wextra
    -Winit-self
    #-Wold-style-cast
    -Woverloaded-virtual
    -Wshadow
    -Wsign-promo
    -Wunreachable-code
    -pedantic
)

add_compile_options(
    "$<$<COMPILE_LANGUAGE:CXX>:${COMMON_CXX_FLAGS}>"
)

INCLUDE_DIRECTORIES(
    source
    source/motion
    source/remote_connection
    source/utils
)

set(MAIN_SOURCES
    source/main.cpp
    source/motion/MotionHandler.cpp
    source/motion/MotorController.cpp
    source/remote_connection/Server.cpp
    source/utils/timer.cpp
)

add_executable(${TARGET_NAME} ${MAIN_SOURCES})
target_link_libraries(${TARGET_NAME} Threads::Threads wiringPi)

