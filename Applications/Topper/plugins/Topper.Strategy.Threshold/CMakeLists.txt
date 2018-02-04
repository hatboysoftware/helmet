cmake_minimum_required(VERSION 3.9)

project (Topper.Strategy.Threshold)

find_package(Helmet REQUIRED)
find_Package (wxWidgets REQUIRED COMPONENTS aui adv base core gl REQUIRED QUIET)
include (${wxWidgets_USE_FILE})
include(wxfreechart-config)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS} ${wxfreechart_INCLUDE_DIRS})

include(Utilities)

set (Topper.Strategy.Threshold_SRC
        src/Configuration.hpp
        src/Module.cpp
        src/Module.hpp
        src/Plugin.cpp
        src/Plugin.hpp
        src/Strategy.cpp
        src/Strategy.hpp
        )

set (Topper.Strategy.Threshold_DEFINITIONS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Topper.Strategy.Threshold_DEFINITIONS})
add_library(Topper.Strategy.Threshold SHARED ${Topper.Strategy.Threshold_SRC})
add_dependencies(Topper.Strategy.Threshold Helmet.Core Helmet.Workbench wxfreechart)
target_link_libraries(Topper.Strategy.Threshold Helmet.Core Helmet.Workbench wxfreechart ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES})
set_target_properties (Topper.Strategy.Threshold PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
