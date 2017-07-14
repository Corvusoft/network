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
add_executable( close_unit_test_suite ${SOURCE_DIR}/close.cpp )
target_link_libraries( close_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( close_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/close_unit_test_suite )

add_executable( consume_unit_test_suite ${SOURCE_DIR}/consume.cpp )
target_link_libraries( consume_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( consume_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/consume_unit_test_suite )

add_executable( create_unit_test_suite ${SOURCE_DIR}/create.cpp )
target_link_libraries( create_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( create_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/create_unit_test_suite )

add_executable( flush_unit_test_suite ${SOURCE_DIR}/flush.cpp )
target_link_libraries( flush_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( flush_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/flush_unit_test_suite )

add_executable( listen_unit_test_suite ${SOURCE_DIR}/listen.cpp )
target_link_libraries( listen_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( listen_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/listen_unit_test_suite )

add_executable( open_unit_test_suite ${SOURCE_DIR}/open.cpp )
target_link_libraries( open_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( open_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/open_unit_test_suite )

add_executable( produce_unit_test_suite ${SOURCE_DIR}/produce.cpp )
target_link_libraries( produce_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( produce_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/produce_unit_test_suite )

add_executable( purge_unit_test_suite ${SOURCE_DIR}/purge.cpp )
target_link_libraries( purge_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( purge_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/purge_unit_test_suite )

add_executable( set_close_handler_unit_test_suite ${SOURCE_DIR}/set_close_handler.cpp )
target_link_libraries( set_close_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_close_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_close_handler_unit_test_suite )

add_executable( set_error_handler_unit_test_suite ${SOURCE_DIR}/set_error_handler.cpp )
target_link_libraries( set_error_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_error_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_error_handler_unit_test_suite )

add_executable( set_message_handler_unit_test_suite ${SOURCE_DIR}/set_message_handler.cpp )
target_link_libraries( set_message_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_message_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_message_handler_unit_test_suite )

add_executable( set_open_handler_unit_test_suite ${SOURCE_DIR}/set_open_handler.cpp )
target_link_libraries( set_open_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_open_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_open_handler_unit_test_suite )

add_executable( setup_unit_test_suite ${SOURCE_DIR}/setup.cpp )
target_link_libraries( setup_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( setup_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/setup_unit_test_suite )

add_executable( teardown_unit_test_suite ${SOURCE_DIR}/teardown.cpp )
target_link_libraries( teardown_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( teardown_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/teardown_unit_test_suite )
