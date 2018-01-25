cmake_minimum_required(VERSION 3.9)

project (Helmet.Core.Tests)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Core.Tests_SRC
    src/plugin.tests.cpp
)

set (Helmet.Core.Tests_DEFINITIONS
        -DBOOST_LOG_DYN_LINK
        -DBOOST_TEST_DYN_LINK
        -DBOOST_TEST_MAIN)

add_definitions(${Helmet.Core.Tests_DEFINITIONS})
add_executable(Helmet.Core.Tests ${Helmet.Core.Tests_SRC})
target_link_libraries(Helmet.Core.Tests Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Core.Tests PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
