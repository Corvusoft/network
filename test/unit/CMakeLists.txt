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
add_executable( tcpip_close_unit_test_suite ${SOURCE_DIR}/tcpip/close.cpp )
target_link_libraries( tcpip_close_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_close_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_close_unit_test_suite )

add_executable( tcpip_constructor_unit_test_suite ${SOURCE_DIR}/tcpip/constructor.cpp )
target_link_libraries( tcpip_constructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_constructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_constructor_unit_test_suite )

add_executable( tcpip_consume_unit_test_suite ${SOURCE_DIR}/tcpip/consume.cpp )
target_link_libraries( tcpip_consume_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_consume_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_consume_unit_test_suite )

add_executable( tcpip_destructor_unit_test_suite ${SOURCE_DIR}/tcpip/destructor.cpp )
target_link_libraries( tcpip_destructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_destructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_destructor_unit_test_suite )

add_executable( tcpip_listen_unit_test_suite ${SOURCE_DIR}/tcpip/listen.cpp )
target_link_libraries( tcpip_listen_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_listen_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_listen_unit_test_suite )

add_executable( tcpip_open_unit_test_suite ${SOURCE_DIR}/tcpip/open.cpp )
target_link_libraries( tcpip_open_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_open_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_open_unit_test_suite )

add_executable( tcpip_produce_unit_test_suite ${SOURCE_DIR}/tcpip/produce.cpp )
target_link_libraries( tcpip_produce_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_produce_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_produce_unit_test_suite )

add_executable( tcpip_setup_unit_test_suite ${SOURCE_DIR}/tcpip/setup.cpp )
target_link_libraries( tcpip_setup_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_setup_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_setup_unit_test_suite )

add_executable( tcpip_teardown_unit_test_suite ${SOURCE_DIR}/tcpip/teardown.cpp )
target_link_libraries( tcpip_teardown_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( tcpip_teardown_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/tcpip_teardown_unit_test_suite )

add_executable( uri_builder_build_unit_test_suite ${SOURCE_DIR}/uri_builder/build.cpp )
target_link_libraries( uri_builder_build_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_build_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_build_unit_test_suite )

add_executable( uri_builder_constructor_unit_test_suite ${SOURCE_DIR}/uri_builder/constructor.cpp )
target_link_libraries( uri_builder_constructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_constructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_constructor_unit_test_suite )

add_executable( uri_builder_destructor_unit_test_suite ${SOURCE_DIR}/uri_builder/destructor.cpp )
target_link_libraries( uri_builder_destructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_destructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_destructor_unit_test_suite )

add_executable( uri_builder_getters_unit_test_suite ${SOURCE_DIR}/uri_builder/getters.cpp )
target_link_libraries( uri_builder_getters_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_getters_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_getters_unit_test_suite )

add_executable( uri_builder_is_finalised_unit_test_suite ${SOURCE_DIR}/uri_builder/is_finalised.cpp )
target_link_libraries( uri_builder_is_finalised_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_is_finalised_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_is_finalised_unit_test_suite )

add_executable( uri_builder_is_malformed_unit_test_suite ${SOURCE_DIR}/uri_builder/is_malformed.cpp )
target_link_libraries( uri_builder_is_malformed_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_is_malformed_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_is_malformed_unit_test_suite )

add_executable( uri_builder_setters_unit_test_suite ${SOURCE_DIR}/uri_builder/setters.cpp )
target_link_libraries( uri_builder_setters_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uri_builder_setters_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/uri_builder_setters_unit_test_suite )
