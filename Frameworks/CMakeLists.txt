cmake_minimum_required(VERSION 3.9)

project (Frameworks)

set (FRAMEWORKS
    Helmet
)

foreach (SUBDIR ${FRAMEWORKS})
    add_subdirectory(${SUBDIR})
endforeach ()