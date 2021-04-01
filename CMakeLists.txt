cmake_minimum_required(VERSION 3.15) #for python find venv cmake function

project(cvMatBind VERSION 0.1)

# Forbid in-source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(
        FATAL_ERROR
	"In source build not permitted"
    )
endif()

option(WITH_TESTS "Build google test project (needs Qt Core)" OFF)
option(WITH_AUTO_PIP "Pip install python library after each build for more comfortable development process" OFF)


# ------------------------------------------------------------------------------
# Test Project
# ------------------------------------------------------------------------------
if(WITH_TESTS)
set(TEST_DIR "${CMAKE_SOURCE_DIR}/src/test")

include("${TEST_DIR}/ConfigTest.cmake" )
endif(WITH_TESTS)
############

# ------------------------------------------------------------------------------
# Python Bindings
# ------------------------------------------------------------------------------
set(PYBIND_DIR "${CMAKE_SOURCE_DIR}/src/pybind")

include("${PYBIND_DIR}/ConfigPyBind.cmake" )
add_subdirectory(${PYBIND_DIR})
############
