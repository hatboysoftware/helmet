cmake_minimum_required(VERSION 3.9)

project (Helmet.Trading)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Trading_HEADERS
        Configuration.hpp
        Exchange/I_Exchange.hpp
        Exchange/I_ExchangePlugin.hpp
        Strategy/I_Strategy.hpp
        Strategy/I_StrategyPlugin.hpp
        )

string(REGEX REPLACE "([^;]+)" "${HELMET_INCLUDE_DIRS}/Helmet/Trading/\\1" Helmet.Trading_HEADERS "${Helmet.Trading_HEADERS}")

set (Helmet.Trading_SRC
        src/Exchange/I_Exchange.cpp
        src/Exchange/I_ExchangePlugin.cpp
        src/Strategy/I_Strategy.cpp
        src/Strategy/I_StrategyPlugin.cpp
        )

set (Helmet.Trading_DEFINITIONS
        -DHELMET_TRADING_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Trading_DEFINITIONS})
add_library(Helmet.Trading SHARED ${Helmet.Trading_SRC} ${Helmet.Trading_HEADERS})
add_dependencies(Helmet.Trading Helmet.Core)
target_link_libraries(Helmet.Trading Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Trading PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

#add_subdirectory(test)