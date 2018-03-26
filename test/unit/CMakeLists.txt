cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "unit test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( close_unit_test_suite ${SOURCE_DIR}/tcpip/close.cpp )
target_link_libraries( close_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( close_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/close_unit_test_suite )

add_executable( constructor_unit_test_suite ${SOURCE_DIR}/tcpip/constructor.cpp )
target_link_libraries( constructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( constructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/constructor_unit_test_suite )

add_executable( consume_unit_test_suite ${SOURCE_DIR}/tcpip/consume.cpp )
target_link_libraries( consume_unit_test_suite ${SHARED_LIBRARY_NAME} )
add_test( consume_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/consume_unit_test_suite )

add_executable( destructor_unit_test_suite ${SOURCE_DIR}/tcpip/destructor.cpp )
target_link_libraries( destructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( destructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/destructor_unit_test_suite )

add_executable( listen_unit_test_suite ${SOURCE_DIR}/tcpip/listen.cpp )
target_link_libraries( listen_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( listen_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/listen_unit_test_suite )

add_executable( open_unit_test_suite ${SOURCE_DIR}/tcpip/open.cpp )
target_link_libraries( open_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( open_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/open_unit_test_suite )

add_executable( produce_unit_test_suite ${SOURCE_DIR}/tcpip/produce.cpp )
target_link_libraries( produce_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( produce_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/produce_unit_test_suite )

add_executable( setup_unit_test_suite ${SOURCE_DIR}/tcpip/setup.cpp )
target_link_libraries( setup_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( setup_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/setup_unit_test_suite )

add_executable( teardown_unit_test_suite ${SOURCE_DIR}/tcpip/teardown.cpp )
target_link_libraries( teardown_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( teardown_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/teardown_unit_test_suite )
