cmake_minimum_required(VERSION 3.9)

project (Topper.Exchange.Kraken)

find_package(Helmet REQUIRED)
find_Package (wxWidgets REQUIRED COMPONENTS aui adv base core gl REQUIRED QUIET)
include (${wxWidgets_USE_FILE})
include(wxfreechart-config)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS} ${wxfreechart_INCLUDE_DIRS})

include(Utilities)

set (Topper.Exchange.Kraken_SRC
        src/Configuration.hpp
        src/Exchange.cpp
        src/Exchange.hpp
        src/Module.cpp
        src/Module.hpp
        src/Plugin.cpp
        src/Plugin.hpp
        )

set (Topper.Exchange.Kraken_DEFINITIONS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Topper.Exchange.Kraken_DEFINITIONS})
add_library(Topper.Exchange.Kraken SHARED ${Topper.Exchange.Kraken_SRC})
add_dependencies(Topper.Exchange.Kraken Helmet.Core Helmet.Trading wxfreechart)
target_link_libraries(Topper.Exchange.Kraken Helmet.Core Helmet.Trading wxfreechart ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES})
set_target_properties (Topper.Exchange.Kraken PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
