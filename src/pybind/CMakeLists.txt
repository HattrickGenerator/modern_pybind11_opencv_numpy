set(PYBIND pybind) 
add_subdirectory(${PYBIND11_DIR})

find_package(OpenCV REQUIRED)
list(APPEND DEPENDENCIES ${OpenCV_LIBS})


set(HEADER_FILES
    matrix-converter.hh
    matrix-converter.inl.hh
matrix-binding.hh
)

set(SOURCE_FILES
module-init.cc
)


set(CVMATBIND_PYTHON_MODULE_NAME cvmatbindpy)

#Use Pybind11 Macro to add module
pybind11_add_module(${CVMATBIND_PYTHON_MODULE_NAME} ${SOURCE_FILES} ${HEADER_FILES})


#####################################################
## Get Virtual environment numpy include directory ##
### Needs to be called after pybind11_add_module ####
#####################################################
#set(Python_FIND_VIRTUALENV ONLY)
find_package (Python COMPONENTS Interpreter Development REQUIRED)
find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED )
set(numpy_include_dir ${Python_SITELIB}/numpy/core/include)
#message("${numpy_include_dir}")
#####################################################

##Dependencies need to be linked private
target_link_libraries(${CVMATBIND_PYTHON_MODULE_NAME} PRIVATE  ${DEPENDENCIES})
target_include_directories(${CVMATBIND_PYTHON_MODULE_NAME} PRIVATE  ${OpenCV_INCLUDE_DIRS} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

include(GNUInstallDirs)

set_target_properties(${CVMATBIND_PYTHON_MODULE_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS ${CVMATBIND_PYTHON_MODULE_NAME}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

