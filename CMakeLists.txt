cmake_minimum_required(VERSION 3.9)

set(PROJ_NAME "cpp_observe")
project(${PROJ_NAME} VERSION 0.1 LANGUAGES CXX)

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Library variables
set(LIB_TARGET_NAME ${PROJ_NAME})
set(LIB_INCLUDE_DIR source/library/include)

# Create library
add_library(${LIB_TARGET_NAME} INTERFACE)
target_include_directories(${LIB_TARGET_NAME} INTERFACE ${LIB_INCLUDE_DIR})
target_compile_features(${LIB_TARGET_NAME} INTERFACE cxx_std_17)

add_library(${LIB_TARGET_NAME}::${LIB_TARGET_NAME} ALIAS ${LIB_TARGET_NAME})




include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

install(TARGETS ${LIB_TARGET_NAME}
	EXPORT ${LIB_TARGET_NAME}-export
	#LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	#ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#set_target_properties(${LIB_TARGET_NAME} PROPERTIES EXPORT_NAME ${LIB_TARGET_NAME})

install(DIRECTORY ${LIB_INCLUDE_DIR}/observe DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${LIB_TARGET_NAME}-targets
	FILE
		${LIB_TARGET_NAME}Targets.cmake
	NAMESPACE
		${LIB_TARGET_NAME}::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/${LIB_TARGET_NAME}ConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	${CMAKE_CURRENT_LIST_DIR}/cmake/${LIB_TARGET_NAME}Config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/${LIB_TARGET_NAME}Config.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/${LIB_TARGET_NAME}Config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/${LIB_TARGET_NAME}ConfigVersion.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)



export(EXPORT ${LIB_TARGET_NAME}-targets
	FILE
		${CMAKE_CURRENT_BINARY_DIR}/${LIB_TARGET_NAME}Targets.cmake
	NAMESPACE
		${LIB_TARGET_NAME}::
)

export(PACKAGE ${LIB_TARGET_NAME})




# Testing variables
set(TEST_TARGET_NAME ${PROJ_NAME}_test)
set(TEST_SOURCE_DIR source/tests/src)
set(TEST_SOURCE_FILES
${TEST_SOURCE_DIR}/test_main.cpp
${TEST_SOURCE_DIR}/observer_tests.cpp
)

# Create testing binary
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(${TEST_TARGET_NAME} ${LIB_TARGET_NAME})
target_link_libraries(${TEST_TARGET_NAME} ${CONAN_LIBS})
target_compile_features(${TEST_TARGET_NAME} PRIVATE cxx_std_17)
