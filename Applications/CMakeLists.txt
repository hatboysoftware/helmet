cmake_minimum_required(VERSION 3.9)

project (Applications)

set (APPLICATIONS
    Topper
)

foreach (SUBDIR ${APPLICATIONS})
    add_subdirectory(${SUBDIR})
endforeach ()