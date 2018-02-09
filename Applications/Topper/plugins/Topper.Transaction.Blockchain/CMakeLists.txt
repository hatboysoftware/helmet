cmake_minimum_required(VERSION 3.9)

project (Topper.Transaction.Blockchain)

find_package(Helmet REQUIRED)
find_Package (wxWidgets REQUIRED COMPONENTS aui adv base core gl REQUIRED QUIET)
include (${wxWidgets_USE_FILE})
include(wxfreechart-config)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS} ${wxfreechart_INCLUDE_DIRS})

include(Utilities)

set (Topper.Transaction.Blockchain_SRC
        src/Configuration.hpp
        src/BlockchainNode.cpp
        src/BlockchainNode.hpp
        src/Module.cpp
        src/Module.hpp
        src/Plugin.cpp
        src/Plugin.hpp
        )

set (Topper.Transaction.Blockchain_DEFINITIONS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Topper.Transaction.Blockchain_DEFINITIONS})
add_library(Topper.Transaction.Blockchain SHARED ${Topper.Transaction.Blockchain_SRC})
add_dependencies(Topper.Transaction.Blockchain Helmet.Core Helmet.Enterprise Helmet.Blockchain wxfreechart)
target_link_libraries(Topper.Transaction.Blockchain Helmet.Core Helmet.Enterprise Helmet.Blockchain wxfreechart ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES})
set_target_properties (Topper.Transaction.Blockchain PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
