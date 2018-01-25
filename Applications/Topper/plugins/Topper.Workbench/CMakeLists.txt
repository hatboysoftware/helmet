cmake_minimum_required(VERSION 3.9)

project (Topper.Workbench)

find_package(Helmet REQUIRED)
find_Package (wxWidgets REQUIRED COMPONENTS aui adv base core gl REQUIRED QUIET)
include (${wxWidgets_USE_FILE})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS})

include(Utilities)

set (Topper.Workbench_SRC
    src/Configuration.hpp
    src/Event.cpp
    src/Event.hpp
    src/Menu.cpp
    src/Menu.hpp
    src/MenuBar.cpp
    src/MenuBar.hpp
    src/MenuItem.cpp
    src/MenuItem.hpp
    src/Module.cpp
    src/Module.hpp
    src/Plugin.cpp
    src/Plugin.hpp
    src/StatusBar.cpp
    src/StatusBar.hpp
    src/TopFrame.cpp
    src/TopFrame.hpp
    src/Workbench.cpp
    src/Workbench.hpp
)

set (Topper.Workbench_DEFINITIONS
        -DTOPPER_WORKBENCH_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Topper.Workbench_DEFINITIONS})
add_library(Topper.Workbench SHARED ${Topper.Workbench_SRC})
add_dependencies(Topper.Workbench Helmet.Core Helmet.Workbench)
target_link_libraries(Topper.Workbench Helmet.Core Helmet.Workbench ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES})
set_target_properties (Topper.Workbench PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
