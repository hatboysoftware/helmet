cmake_minimum_required(VERSION 3.9)

project (Helmet.Application)

find_package(HelmetCore REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_CORE_INCLUDE_DIR})

include(Utilities)

find_source_files(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
set (Helmet.Application_SRC ${SOURCE_FILES})
source_group_by_folder(Helmet.Application ${CMAKE_CURRENT_SOURCE_DIR})

set (Helmet.Application_DEFINITIONS
        -DHELMET_CORE_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Application_DEFINITIONS})
add_library(Helmet.Application SHARED ${Helmet.Application_SRC})
target_link_libraries(Helmet.Application Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Application PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
