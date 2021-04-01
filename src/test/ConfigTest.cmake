set(TEST_LIBS_DIR "${CMAKE_SOURCE_DIR}/src/test/lib")

set(GTEST_RECIPE_DIR googletest-recipe)
set(GTEST_SOURCE_DIR googletest-src)

################################# UNIT TESTS ########################################
#Set up Google test installation
#Thanks to: https://chromium.googlesource.com/external/github.com/google/googletest/+/HEAD/googletest/README.md

# Download and unpack googletest at configure time
if (NOT EXISTS ${TEST_LIBS_DIR}/${GTEST_RECIPE_DIR}/CMakeLists.txt)
    configure_file(${TEST_DIR}/CMakeLists.txt.gtest ${TEST_LIBS_DIR}/${GTEST_RECIPE_DIR}/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${TEST_LIBS_DIR}/${GTEST_RECIPE_DIR}"
        RESULT_VARIABLE result)
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND "${CMAKE_COMMAND}" --build . WORKING_DIRECTORY "${TEST_LIBS_DIR}/${GTEST_RECIPE_DIR}")
endif()

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory("${TEST_LIBS_DIR}/${GTEST_SOURCE_DIR}")
add_subdirectory(${TEST_DIR})
