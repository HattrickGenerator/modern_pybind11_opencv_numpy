set(UNIT_TESTS unit_tests)


find_package(Qt5 COMPONENTS Core REQUIRED)
list(APPEND DEPENDENCIES Qt5::Core )

set(SOURCES
    test-main.cc
    python-import-test.cc
)

add_executable(${UNIT_TESTS} ${SOURCES})

target_link_libraries(${UNIT_TESTS} gtest gtest_main ${EXE_NAME_LIB} ${DEPENDENCIES})
set_target_properties(${UNIT_TESTS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

    set(TEST_FILE_DIR pytestfiles)

    file(GLOB PYTHON_TEST_SCRIPTS
      ${CMAKE_CURRENT_LIST_DIR}/${TEST_FILE_DIR}
    )

foreach(file ${PYTHON_TEST_SCRIPTS})
	message(${file})
	file(COPY ${file} DESTINATION ${CMAKE_BINARY_DIR})
endforeach(file)

