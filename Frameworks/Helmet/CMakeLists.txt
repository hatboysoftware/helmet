cmake_minimum_required(VERSION 3.9)

project (Frameworks.Helmet)

set (MODULES
    Core
    Enterprise
    Workbench
    Blockchain
    Trading
)

foreach (SUBDIR ${MODULES})
    add_subdirectory(${SUBDIR})
endforeach ()