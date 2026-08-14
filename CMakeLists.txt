cmake_minimum_required(VERSION 3.28)

project(Otuber VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OTUBER_BUILD_TESTS "Build Otuber tests" ON)

# The first milestone keeps the project dependency-light. OBS integration is
# isolated behind src/obs so the core can be developed and tested separately.
add_library(otuber_core STATIC
    src/core/avatar/avatar_model.cpp
    src/core/avatar/avatar_model.hpp
    src/core/motion/motion_state.cpp
    src/core/motion/motion_state.hpp
)

target_include_directories(otuber_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(OTUBER_BUILD_TESTS)
    enable_testing()
    add_executable(otuber_core_tests
        tests/core/avatar_model_test.cpp
        tests/core/motion_state_test.cpp
    )
    target_link_libraries(otuber_core_tests PRIVATE otuber_core)
    add_test(NAME otuber_core_tests COMMAND otuber_core_tests)
endif()

# OBS plugin target will be added once the platform-specific OBS SDK setup is
# established. Keeping the core independent prevents OBS headers from leaking
# into the avatar model and motion APIs.
