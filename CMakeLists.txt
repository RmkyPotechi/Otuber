cmake_minimum_required(VERSION 3.20)

project(Otuber VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OTUBER_BUILD_TESTS "Build Otuber core smoke tests" ON)

add_library(otuber_core STATIC
    src/core/avatar/parameter_mapper.cpp
    src/core/avatar/parameter_mapper.hpp
    src/core/avatar/avatar_format.hpp
    src/core/mesh/mesh.cpp
    src/core/mesh/mesh.hpp
    src/core/mesh/deformation.cpp
    src/core/mesh/deformation.hpp
    src/core/mesh/deformer.cpp
    src/core/mesh/deformer.hpp
    src/core/generation/generation_pipeline.cpp
    src/core/generation/generation_pipeline.hpp
    src/core/generation/generated_assets.hpp
)

target_include_directories(otuber_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(OTUBER_BUILD_TESTS)
    enable_testing()

    add_executable(otuber_core_tests
        tests/core/core_smoke_test.cpp
    )

    target_link_libraries(otuber_core_tests PRIVATE otuber_core)

    add_test(NAME otuber_core_tests COMMAND otuber_core_tests)
endif()

# OBS integration will be added after the core smoke-test milestone.
# Keeping the engine dependency-free makes deformation, mapping, and
# generation preparation testable without launching OBS.
