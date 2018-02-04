cmake_minimum_required(VERSION 3.9)

project (ThirdParty)

set (THIRDPARTYLIBS
    wxfreechart
)

foreach (SUBDIR ${THIRDPARTYLIBS})
    add_subdirectory(${SUBDIR})
endforeach ()