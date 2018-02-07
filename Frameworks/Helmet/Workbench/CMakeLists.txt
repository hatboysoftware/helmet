cmake_minimum_required(VERSION 3.9)

project (Helmet.Workbench)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Workbench_HEADERS
        Configuration.hpp
        I_Controller.hpp
        I_Entity.hpp
        I_Event.hpp
        I_Frame.hpp
        I_Menu.hpp
        I_MenuBar.hpp
        I_MenuItem.hpp
        I_Model.hpp
        I_Notebook.hpp
        I_NotebookPage.hpp
        I_StatusBar.hpp
        I_View.hpp
        I_Workbench.hpp
        I_WorkbenchPlugin.hpp
)

string(REGEX REPLACE "([^;]+)" "${HELMET_INCLUDE_DIRS}/Helmet/Workbench/\\1" Helmet.Workbench_HEADERS "${Helmet.Workbench_HEADERS}")

set (Helmet.Workbench_SRC
    src/I_Controller.cpp
    src/I_Entity.cpp
    src/I_Event.cpp
    src/I_Frame.cpp
    src/I_Menu.cpp
    src/I_MenuBar.cpp
    src/I_MenuItem.cpp
    src/I_Model.cpp
    src/I_Notebook.cpp
    src/I_NotebookPage.cpp
    src/I_StatusBar.cpp
    src/I_View.cpp
    src/I_Workbench.cpp
    src/I_WorkbenchPlugin.cpp
)

set (Helmet.Workbench_DEFINITIONS
        -DHELMET_WORKBENCH_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Workbench_DEFINITIONS})
add_library(Helmet.Workbench SHARED ${Helmet.Workbench_SRC} ${Helmet.Workbench_HEADERS})
add_dependencies(Helmet.Workbench Helmet.Core)
target_link_libraries(Helmet.Workbench Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Workbench PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
