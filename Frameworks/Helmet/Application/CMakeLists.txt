cmake_minimum_required(VERSION 3.9)

project (Helmet.Application)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Application_SRC
    src/I_Application.cpp
    src/I_Window.cpp
)

set (Helmet.Application_DEFINITIONS
        -DHELMET_APPLICATION_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Application_DEFINITIONS})
add_library(Helmet.Application SHARED ${Helmet.Application_SRC})
add_dependencies(Helmet.Application Helmet.Core)
target_link_libraries(Helmet.Application Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Application PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
