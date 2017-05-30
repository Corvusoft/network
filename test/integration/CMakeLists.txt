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
add_executable( tcpip_adaptor_integration_test_suite ${SOURCE_DIR}/tcpip_adaptor.cpp )
target_link_libraries( tcpip_adaptor_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_adaptor_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_adaptor_integration_test_suite )
