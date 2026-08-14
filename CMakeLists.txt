cmake_minimum_required(VERSION 3.20)

project(Otuber VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OTUBER_BUILD_TESTS "Build Otuber core smoke tests" ON)
option(OTUBER_ENABLE_OPENCV_TRACKER "Build the optional OpenCV face tracker" ON)

add_library(otuber_core STATIC
    src/core/avatar/parameter_mapper.cpp
    src/core/avatar/parameter_mapper.hpp
    src/core/avatar/avatar_format.hpp
    src/core/avatar/avatar_instance.cpp
    src/core/avatar/avatar_instance.hpp
    src/core/mesh/mesh.cpp
    src/core/mesh/mesh.hpp
    src/core/mesh/deformation.cpp
    src/core/mesh/deformation.hpp
    src/core/mesh/deformer.cpp
    src/core/mesh/deformer.hpp
    src/core/mesh/deformer_graph.cpp
    src/core/mesh/deformer_graph.hpp
    src/core/generation/generation_pipeline.cpp
    src/core/generation/generation_pipeline.hpp
    src/core/generation/generated_assets.hpp
    src/core/texture/texture.cpp
    src/core/texture/texture.hpp
    src/core/motion/camera_tracker.cpp
    src/core/motion/camera_tracker.hpp
    src/core/motion/camera_state.hpp
    src/core/motion/landmark_mapper.cpp
    src/core/motion/landmark_mapper.hpp
    src/core/motion/landmarks.hpp
    src/core/motion/face_tracker.hpp
)

target_include_directories(otuber_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(OTUBER_ENABLE_OPENCV_TRACKER)
    find_package(OpenCV QUIET COMPONENTS core imgproc objdetect)
    if(OpenCV_FOUND)
        add_library(otuber_opencv_tracker STATIC
            src/platform/windows/tracking/opencv_face_tracker.cpp
            src/platform/windows/tracking/opencv_face_tracker.hpp
        )
        target_include_directories(otuber_opencv_tracker PRIVATE ${OpenCV_INCLUDE_DIRS})
        target_link_libraries(otuber_opencv_tracker PRIVATE otuber_core ${OpenCV_LIBS})
        target_compile_definitions(otuber_opencv_tracker PRIVATE OTUBER_HAS_OPENCV_TRACKER=1)
    else()
        message(STATUS "OpenCV not found: OpenCV face tracker will not be built")
    endif()
endif()

if(OTUBER_BUILD_TESTS)
    enable_testing()

    add_executable(otuber_core_tests
        tests/core/core_smoke_test.cpp
    )

    target_link_libraries(otuber_core_tests PRIVATE otuber_core)

    add_test(NAME otuber_core_tests COMMAND otuber_core_tests)
endif()

# OBS integration will be added after the core and motion milestones.
# Keeping capture/tracking backends separate from the core makes them replaceable.
