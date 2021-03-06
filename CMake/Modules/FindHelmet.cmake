if (NOT HELMET_INCLUDE_DIRS)
    set (HELMET_INCLUDE_DIRS)
endif()

list (APPEND HELMET_INCLUDE_DIRS ${ROOT_SOURCE_DIR}/Frameworks/include)
list (REMOVE_DUPLICATES HELMET_INCLUDE_DIRS)

list (APPEND HELMET_LIBRARY_DIRS ${ROOT_BINARY_DIR}/lib)
list (REMOVE_DUPLICATES HELMET_LIBRARY_DIRS)

mark_as_advanced(HELMET_INCLUDE_DIRS HELMET_LIBRARY_DIRS)
