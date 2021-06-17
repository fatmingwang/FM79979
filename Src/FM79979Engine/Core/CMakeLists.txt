cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_subdirectory(Android)
add_subdirectory(Bluetooth)
add_subdirectory(Common)
add_subdirectory(GameplayUT)
add_subdirectory(Math)
add_subdirectory(Network)
add_subdirectory(Sound)
add_subdirectory(Synchronization)
add_subdirectory(XML)


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#target_include_directories(Tutorial PUBLIC "${PROJECT_BINARY_DIR}")
