cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "integration test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( create_integration_test_suite ${SOURCE_DIR}/create.cpp )
target_link_libraries( create_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( create_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/create_integration_test_suite )

add_executable( listen_integration_test_suite ${SOURCE_DIR}/listen.cpp )
target_link_libraries( listen_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( listen_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/listen_integration_test_suite )

add_executable( open_integration_test_suite ${SOURCE_DIR}/open.cpp )
target_link_libraries( open_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( open_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/open_integration_test_suite )

add_executable( setup_integration_test_suite ${SOURCE_DIR}/setup.cpp )
target_link_libraries( setup_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( setup_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/setup_integration_test_suite )
