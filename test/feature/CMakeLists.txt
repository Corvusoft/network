cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "feature test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( bi_directional_communication_feature_test_suite ${SOURCE_DIR}/bi_directional_communication.cpp )
target_link_libraries( bi_directional_communication_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( bi_directional_communication_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/bi_directional_communication_feature_test_suite )

add_executable( service_handler_callback_order_feature_test_suite ${SOURCE_DIR}/service_handler_callback_order.cpp )
target_link_libraries( service_handler_callback_order_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( service_handler_callback_order_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/service_handler_callback_order_feature_test_suite )

add_executable( uni_directional_communication_feature_test_suite ${SOURCE_DIR}/uni_directional_communication.cpp )
target_link_libraries( uni_directional_communication_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( uni_directional_communication_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/uni_directional_communication_feature_test_suite )
