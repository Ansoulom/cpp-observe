cmake_minimum_required(VERSION 3.9)
project(cpp_observe VERSION 0.2.0 LANGUAGES CXX)

# Create library
add_library(cpp_observe INTERFACE)
target_include_directories(cpp_observe
	INTERFACE
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/library/include>
)
target_compile_features(cpp_observe INTERFACE cxx_std_17)

add_library(cpp_observe::cpp_observe ALIAS cpp_observe)


# Install
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_PREFIX})

install(TARGETS cpp_observe
	EXPORT cpp_observe-targets
)

install(DIRECTORY source/library/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT cpp_observe-targets
	FILE
		cpp_observeTargets.cmake
	NAMESPACE
		cpp_observe::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/cpp_observeConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	${CMAKE_CURRENT_LIST_DIR}/cmake/cpp_observeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/cpp_observeConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
	${CMAKE_SOURCE_DIR}/LICENSE
	${CMAKE_CURRENT_BINARY_DIR}/cpp_observeConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/cpp_observeConfigVersion.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)

# Export
export(EXPORT cpp_observe-targets
	FILE
		${CMAKE_CURRENT_BINARY_DIR}/cpp_observeTargets.cmake
	NAMESPACE
		cpp_observe::
)
export(PACKAGE cpp_observe)

# Testing
option(BUILD_TESTS "Build with tests" OFF)
if(BUILD_TESTS)

add_executable(cpp_observe_test
source/tests/src/test_main.cpp
source/tests/src/observer_tests.cpp
)

find_package(Catch2 REQUIRED)

target_link_libraries(cpp_observe_test cpp_observe)
target_link_libraries(cpp_observe_test Catch2::Catch2)
target_compile_features(cpp_observe_test PRIVATE cxx_std_17)

endif()
