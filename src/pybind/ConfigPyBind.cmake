	

set(PYBIND_LIBS_DIR "${PYBIND_DIR}/lib")

set(PYBIND11_DIR ${PYBIND_LIBS_DIR}/pybind11)
set(PYBIND_RECIPE_DIR ${PYBIND_LIBS_DIR}/pybind-recipe)

################################# PyBind ########################################

# Download and unpack Pybind11 at configure time
if (NOT EXISTS ${PYBIND11_DIR}/CMakeLists.txt)
    configure_file(${PYBIND_DIR}/CMakeLists.txt.pybind ${PYBIND11_DIR}/CMakeLists.txt)
	
	#make directory if it doesn't already exist
	if (NOT EXISTS ${PYBIND_RECIPE_DIR})
		file(MAKE_DIRECTORY ${PYBIND_RECIPE_DIR})
	endif()
	
	#CMake the generated file
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" ${PYBIND11_DIR}
        WORKING_DIRECTORY ${PYBIND_RECIPE_DIR}
        RESULT_VARIABLE result)
    if(result)
        message(FATAL_ERROR "CMake step for pybind failed: ${result}")
    endif()
    
    execute_process(COMMAND "${CMAKE_COMMAND}" --build . WORKING_DIRECTORY "${PYBIND_RECIPE_DIR}")
	
endif()

