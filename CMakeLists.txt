cmake_minimum_required(VERSION 3.9)
project(Helmet CXX)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system locale filesystem thread log unit_test_framework REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

if (NOT ROOT_SOURCE_DIR)
    set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif (NOT ROOT_SOURCE_DIR)

if (NOT ROOT_BINARY_DIR)
    set(ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT ROOT_BINARY_DIR)

set (CMAKE_MODULE_PATH
        ${ROOT_SOURCE_DIR}/CMake/Modules
        ${ROOT_BINARY_DIR}/3rdParty/wxfreechart
        ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

set (COMPONENTS
    3rdParty
    Frameworks
    Applications
)

foreach (SUBDIR ${COMPONENTS})
    add_subdirectory(${SUBDIR})
endforeach()

# TODO Generate Installer project