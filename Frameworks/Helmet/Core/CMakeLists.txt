cmake_minimum_required(VERSION 3.9)

project (Helmet.Core)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Core_SRC
    src/Plugin/Environment.cpp
    src/Plugin/Environment.hpp
    src/Plugin/I_Environment.cpp
    src/Plugin/I_Module.cpp
    src/Plugin/I_ModuleInfo.cpp
    src/Plugin/I_ModuleManager.cpp
    src/Plugin/I_ModuleService.cpp
    src/Plugin/I_Plugin.cpp
    src/Plugin/I_PluginManager.cpp
    src/Plugin/ModuleInfo.cpp
    src/Plugin/ModuleInfo.hpp
    src/Plugin/ModuleManager.cpp
    src/Plugin/ModuleManager.hpp
    src/Plugin/ModuleService.cpp
    src/Plugin/ModuleService.hpp
    src/Plugin/PluginManager.cpp
    src/Plugin/PluginManager.hpp
)

set (Helmet.Core_DEFINITIONS
    -DHELMET_CORE_EXPORTS
    -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Core_DEFINITIONS})
add_library(Helmet.Core SHARED ${Helmet.Core_SRC})
target_link_libraries(Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Core PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)