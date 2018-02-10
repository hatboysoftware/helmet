cmake_minimum_required(VERSION 3.9)

project (Helmet.Core)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set(Helmet.Core_HEADER_PREFIX ${HELMET_INCLUDE_DIRS}/Helmet/Core/)

set(Helmet.Core_HEADERS
        Configuration.hpp
        Event/I_Action.hpp
        Event/I_Connection.hpp
        Event/I_Event.hpp
        Event/I_Observable.hpp
        Event/I_Observer.hpp
        Exception/Exception.hpp
        Exception/NotImplementedException.hpp
        Plugin/I_Environment.hpp
        Plugin/I_Module.hpp
        Plugin/I_ModuleInfo.hpp
        Plugin/I_ModuleManager.hpp
        Plugin/I_ModuleService.hpp
        Plugin/I_Plugin.hpp
        Plugin/I_PluginManager.hpp
        Thread/ConditionFactory.hpp
        Thread/CriticalSection.hpp
        Thread/I_Callable.hpp
        Thread/I_Condition.hpp
        Thread/I_Mutex.hpp
        Thread/I_Runnable.hpp
        Thread/I_Thread.hpp
        Thread/MutexFactory.hpp
        Thread/SynchronizedQueue.hpp
        Thread/ThreadFactory.hpp
        Thread/ThreadPool.hpp
        Utility/Deleter.hpp
        Utility/I_Cleanable.hpp
        Utility/I_Handle.hpp
        Utility/I_Resource.hpp
        Utility/I_Visitor.hpp
        Utility/Utility.hpp
)

string(REGEX REPLACE "([^;]+)" "${HELMET_INCLUDE_DIRS}/Helmet/Core/\\1" Helmet.Core_HEADERS "${Helmet.Core_HEADERS}")

set (Helmet.Core_SRC
        src/Event/I_Action.cpp
        src/Event/I_Connection.cpp
        src/Event/I_Event.cpp
        src/Event/I_Observable.cpp
        src/Event/I_Observer.cpp
        src/Exception/Exception.cpp
        src/Exception/NotImplementedException.cpp
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
        src/Thread/Condition.cpp
        src/Thread/Condition.hpp
        src/Thread/ConditionFactory.cpp
        src/Thread/CriticalSection.cpp
        src/Thread/I_Condition.cpp
        src/Thread/I_Mutex.cpp
        src/Thread/I_Thread.cpp
        src/Thread/Mutex.cpp
        src/Thread/Mutex.hpp
        src/Thread/MutexFactory.cpp
        src/Thread/SynchronizedQueue.cpp
        src/Thread/Thread.cpp
        src/Thread/Thread.hpp
        src/Thread/ThreadFactory.cpp
        src/Thread/ThreadPool.cpp
        src/Utility/I_Cleanable.cpp
)

set (Helmet.Core_DEFINITIONS
        -DHELMET_CORE_EXPORTS
        -DBOOST_LOG_DYN_LINK
)

add_definitions(${Helmet.Core_DEFINITIONS})
add_library(Helmet.Core SHARED ${Helmet.Core_SRC} ${Helmet.Core_HEADERS})
target_link_libraries(Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Core PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)