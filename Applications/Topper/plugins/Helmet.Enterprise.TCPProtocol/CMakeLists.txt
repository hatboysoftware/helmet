cmake_minimum_required(VERSION 3.9)

project (Helmet.Enterprise.TCPProtocol)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS} ${wxfreechart_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Enterprise.TCPProtocol_SRC
        src/Configuration.hpp
        src/ProtocolService.cpp
        src/ProtocolService.hpp
        src/Module.cpp
        src/Module.hpp
        src/Plugin.cpp
        src/Plugin.hpp
        )

set (Helmet.Enterprise.TCPProtocol_DEFINITIONS
        -DHELMET_PROTOCOL_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Enterprise.TCPProtocol_DEFINITIONS})
add_library(Helmet.Enterprise.TCPProtocol SHARED ${Helmet.Enterprise.TCPProtocol_SRC})
add_dependencies(Helmet.Enterprise.TCPProtocol Helmet.Core Helmet.Enterprise)
target_link_libraries(Helmet.Enterprise.TCPProtocol Helmet.Core Helmet.Enterprise ${Boost_LIBRARIES})
set_target_properties (Helmet.Enterprise.TCPProtocol PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
