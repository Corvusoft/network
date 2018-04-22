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
add_executable( uri_accessors_build_integration_test_suite ${SOURCE_DIR}/uri_builder/accessors.cpp )
target_link_libraries( uri_accessors_build_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_accessors_build_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_accessors_build_integration_test_suite )

add_executable( uri_builder_build_integration_test_suite ${SOURCE_DIR}/uri_builder/build.cpp )
target_link_libraries( uri_builder_build_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_build_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_build_integration_test_suite )

add_executable( uri_builder_constructor_integration_test_suite ${SOURCE_DIR}/uri_builder/constructor.cpp )
target_link_libraries( uri_builder_constructor_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_constructor_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_constructor_integration_test_suite )

add_executable( uri_builder_is_finalised_integration_test_suite ${SOURCE_DIR}/uri_builder/is_finalised.cpp )
target_link_libraries( uri_builder_is_finalised_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_is_finalised_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_is_finalised_integration_test_suite )

add_executable( uri_builder_is_malformed_integration_test_suite ${SOURCE_DIR}/uri_builder/is_malformed.cpp )
target_link_libraries( uri_builder_is_malformed_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_is_malformed_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_is_malformed_integration_test_suite )
