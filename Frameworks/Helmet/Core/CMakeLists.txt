cmake_minimum_required(VERSION 3.9)

project (Helmet.Core)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

include(Utilities)

find_source_files(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
set (Helmet.Core_SRC ${SOURCE_FILES})
source_group_by_folder(Helmet.Core ${CMAKE_CURRENT_SOURCE_DIR})

set (Helmet.Core_DEFINITIONS
    -DHELMET_CORE_EXPORTS
    -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Core_DEFINITIONS})
add_library(Helmet.Core SHARED ${Helmet.Core_SRC})
target_link_libraries(Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Core PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
