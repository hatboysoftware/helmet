cmake_minimum_required(VERSION 3.9)

project (Topper)

find_package(Helmet REQUIRED)
find_Package (wxWidgets REQUIRED COMPONENTS aui adv base core gl REQUIRED QUIET)
include (${wxWidgets_USE_FILE})

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS})

include(Utilities)

set (Topper_SRC
    src/Application.cpp
    src/Application.hpp
)

set (Topper_DEFINITIONS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Topper_DEFINITIONS})
add_executable(Topper ${Topper_SRC})
target_link_libraries(Topper Helmet.Core Helmet.Workbench ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES})
set_target_properties (Topper PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(plugins)