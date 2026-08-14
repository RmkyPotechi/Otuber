cmake_minimum_required(VERSION 3.20)
project(otuber_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(otuber_preview main.cpp)
target_include_directories(otuber_preview PRIVATE ../../src)

# The preview currently runs without a platform GUI dependency. It emits
# transformed mesh data so the core can be validated before adding a GPU/UI backend.
