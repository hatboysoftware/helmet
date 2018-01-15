MACRO(source_group_by_folder target root)
    SET(SOURCE_GROUP_DELIMITER "/")
    SET(last_dir "")
    SET(files "")
    FOREACH(file ${${target}_SRC} ${${target}_HEADERS})
        file(RELATIVE_PATH relative_file "${root}" ${file})
        GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
        IF (NOT "${dir}" STREQUAL "${last_dir}")
            IF (files)
                SOURCE_GROUP("${last_dir}" FILES ${files})
            ENDIF (files)
            SET(files "")
        ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
        SET(files ${files} ${file})
        SET(last_dir "${dir}")
    ENDFOREACH(file)
    IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
    ENDIF (files)
ENDMACRO(source_group_by_folder)

function (join_list _output _separator)
    set (result "")
    set (separator "")
    foreach (element ${ARGN})
        set (result "${result}${separator}${element}")
        set (separator "${_separator}")
    endforeach ()
    set (${_output} "${result}" PARENT_SCOPE)
endfunction (join_list)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

# Function:                 exclude_files_from_dir_in_list
# Description:              Exclude all files from a list under a specific directory.
# Param _InFileList:        Input and returned List
# Param _excludeDirName:    Name of the directory, which shall be ignored.
# Param _verbose:           Print the names of the files handled

FUNCTION (exclude_files_from_dir_in_list _InFileList _excludeDirName _verbose)
    foreach (ITR ${_InFileList})
        if ("${_verbose}")
            message(STATUS "ITR=${ITR}")
        endif ("${_verbose}")

        if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")                   # Check if the item matches the directory name in _excludeDirName
            message(STATUS "Remove Item from List:${ITR}")
            list (REMOVE_ITEM _InFileList ${ITR})                              # Remove the item from the list
        endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

    endforeach(ITR)
    set(SOURCE_FILES ${_InFileList} PARENT_SCOPE)                          # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (exclude_files_from_dir_in_list)

FUNCTION (find_source_files _SourceDir _SourceFiles)
    set (SOURCE_FILES ${${_SourceFiles}})
    file (GLOB SUB_FOLDERS RELATIVE ${_SourceDir} ${_SourceDir}/*)
    foreach (SUB_FOLDER ${SUB_FOLDERS})
        if (IS_DIRECTORY ${_SourceDir}/${SUB_FOLDER} AND NOT ${SUB_FOLDER} MATCHES "Tests")
            find_source_files(${_SourceDir}/${SUB_FOLDER} SOURCE_FILES)
        else ()
            file (GLOB Source_H ${_SourceDir}/*.h)
            file (GLOB Source_HPP ${_SourceDir}/*.hpp)
            file (GLOB Source_CPP ${_SourceDir}/*.cpp)
            list (APPEND SOURCE_FILES ${Source_H} ${Source_HPP} ${Source_CPP})
        endif ()
    endforeach (SUB_FOLDER)
    set (${_SourceFiles} ${SOURCE_FILES} PARENT_SCOPE)
ENDFUNCTION (find_source_files)

FUNCTION (find_cuda_files _SourceDir _SourceFiles)
    set (CUDA_FILES ${${_SourceFiles}})
    file (GLOB SUB_FOLDERS RELATIVE ${_SourceDir} ${_SourceDir}/*)
    foreach (SUB_FOLDER ${SUB_FOLDERS})
        if (IS_DIRECTORY ${_SourceDir}/${SUB_FOLDER} AND NOT ${SUB_FOLDER} MATCHES "Tests")
            find_cuda_files(${_SourceDir}/${SUB_FOLDER} CUDA_FILES)
        else ()
            file (GLOB Source_CU ${_SourceDir}/*.cu)
            list (APPEND CUDA_FILES ${Source_CU})
        endif ()
    endforeach (SUB_FOLDER)
    set (${_SourceFiles} ${CUDA_FILES} PARENT_SCOPE)
ENDFUNCTION (find_cuda_files)

function (find_include_files _include_files _relative_path _current_folder)
    file (GLOB SUB_FOLDERS RELATIVE "${_relative_path}/${_current_folder}" "${_relative_path}/${_current_folder}/*")
    foreach (SUB_FOLDER ${SUB_FOLDERS})
        if (IS_DIRECTORY "${_relative_path}/${_current_folder}/${SUB_FOLDER}")
            find_include_files(${_include_files} ${_relative_path} "${_current_folder}/${SUB_FOLDER}")
        endif ()
    endforeach ()

    set (INCLUDE_FILES ${${_include_files}})
    list (APPEND INCLUDE_FILES ${_current_folder}/*.h)
    list (APPEND INCLUDE_FILES ${_current_folder}/*.hpp)
    list (APPEND INCLUDE_FILES ${_current_folder}/*.c)
    list (APPEND INCLUDE_FILES ${_current_folder}/*.cpp)
    set (${_include_files} ${INCLUDE_FILES} PARENT_SCOPE)
endfunction (find_include_files)

function (install_header_files _headerDir _installDir)
    set (_headerFiles)
    file (GLOB SUB_FOLDERS RELATIVE ${_headerDir} ${_headerDir}/*)
    foreach (ENTRY ${SUB_FOLDERS})
        if (IS_DIRECTORY ${_headerDir}/${ENTRY})
            install_header_files (${_headerDir}/${ENTRY} ${_installDir}/${ENTRY})
        else ()
            list (APPEND _headerFiles ${_headerDir}/${ENTRY})
        endif ()
    endforeach(ENTRY)
    install (FILES ${_headerFiles} DESTINATION ${_installDir})
endfunction (install_header_files)

function (copy_header_files _target _headerDir _installDir)
    get_filename_component(installDir ${_installDir} ABSOLUTE)
    if (NOT EXISTS ${installDir})
        file (MAKE_DIRECTORY ${installDir})
    endif ()
    file (GLOB SUB_FOLDERS RELATIVE ${_headerDir} ${_headerDir}/*)
    foreach (ENTRY ${SUB_FOLDERS})
        if (IS_DIRECTORY ${_headerDir}/${ENTRY})
            copy_header_files (${_target} ${_headerDir}/${ENTRY} ${_installDir}/${ENTRY})
        else ()
            add_custom_command(TARGET ${_target} PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_headerDir}/${ENTRY} ${_installDir}/${ENTRY})
        endif ()
    endforeach(ENTRY)
endfunction (copy_header_files)

function (generate_nuspec_autopkg _nuspec _name)

    join_list (NUSPEC_AUTHORS ",\n            " ${${_name}_AUTHORS})

    join_list (NUSPEC_OWNERS ",\n            " ${${_name}_OWNERS})

    join_list (NUSPEC_TAGS ",\n            " ${${_name}_TAGS})

    set (NUSPEC_HEADER
            "    nuspec {\n\
        id = ${${_name}_ID};\n\
        version: ${${_name}_VERSION};\n\
        title: ${${_name}_TITLE};\n\
        authors: {\n\
            ${NUSPEC_AUTHORS}\n\
        };\n\
        owners: {\n\
            ${NUSPEC_OWNERS}\n\
        };\n\
        licenseUrl: \"${${_name}_LICENSE_URL}\";\n\
        iconUrl: \"${${_name}_ICON_URL}\";\n\
        requireLicenseAcceptance: ${${_name}_REQUIRE_LICENSE_ACCEPTANCE};\n\
        summary: ${${_name}_SUMMARY};\n\
        description: @\"${${_name}_DESCRIPTION}\";\n\
        releaseNotes: @\"${${_name}_RELEASE_NOTES}\";\n\
        projectUrl: \"${${_name}_PROJECT_URL}\";\n\
        copyright: ${${_name}_COPYRIGHT};\n\
        tags: {\n\
            ${NUSPEC_TAGS}\n\
        };\n\
    }\n\
\n")

    join_list (NUSPEC_PACKAGES ",\n\
            " ${${_name}_PACKAGES})
    set (NUSPEC_DEPENDENCIES "\
    dependencies {\n\
        packages: {\n\
            ${NUSPEC_PACKAGES}\n\
        };\n\
    }\n\
\n")

    set (NUSPEC_INCLUDE "")
    if (${_name}_INCLUDE_PATH_PREFIX)
        string (REGEX REPLACE "/" "\\\\" NATIVE_INCLUDE_PATH_PREFIX ${${_name}_NATIVE_INCLUDE_PATH_PREFIX})
        set (NUSPEC_INCLUDE "${NUSPEC_INCLUDE}\
        nestedInclude: {\n\
            #destination = \${d_include}${${_name}_INCLUDE_PATH_PREFIX};\n\
            ${NATIVE_INCLUDE_PATH_PREFIX}\\**\\*\n\
        };\n\
\n\
        include: {\n\
            \"VERSION\"\n\
        };\n\
\n")
    else ()
        set (NUSPEC_INCLUDE "${NUSPEC_INCLUDE}\
        nestedInclude: {\n\
            #destination = \${d_include};\n\
            include\\**\\*\n\
        };\n\
\n\
        include: {\n\
            \"VERSION\"\n\
        };\n\
\n")
    endif ()

    set (NUSPEC_CONFIGURATIONS "")
    foreach (NUSPEC_PLATFORM ${${_name}_PLATFORMS})
        foreach (NUSPEC_TOOLSET ${${_name}_TOOLSETS})
            foreach (NUSPEC_CONFIGURATION ${${_name}_CONFIGURATIONS})
                foreach (NUSPEC_LINKAGE ${${_name}_LINKAGES})
                    set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "\
        [${NUSPEC_PLATFORM},${NUSPEC_TOOLSET},${NUSPEC_CONFIGURATION},${NUSPEC_LINKAGE}] {\n")

                    if (${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_LIBS)
                        join_list (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_LIBS
                                ",\n\
                "
                                ${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_LIBS})
                        set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}\
            lib: {\n\
                ${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_LIBS}\n\
            };\n")
                    endif ()

                    if (${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_LIB_PATH_PREFIX)
                        string (REGEX REPLACE
                                "/" "\\\\"
                                NATIVE_LIB_PATH_PREFIX ${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_NATIVE_LIB_PATH_PREFIX})

                        set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}\
            nestedLib: {\n\
                #destination = ${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_LIB_PATH_PREFIX};\n\
                ${NATIVE_LIB_PATH_PREFIX}\\**\\*\n\
            };\n")
                    endif ()

                    if (${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_SYMBOLS)
                        join_list (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_SYMBOLS
                                ",\n\
                "
                                ${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_SYMBOLS})
                        set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}\
            symbols: {\n\
                ${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_SYMBOLS}\n\
            };\n")
                    endif ()

                    if (${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_BINS)
                        join_list (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_BINS
                                ",\n\
                "
                                ${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_BINS})
                        set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}\
            bin: {\n\
                ${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_BINS}\n\
            };\n")
                    endif ()

                    if (${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_BIN_PATH_PREFIX)
                        string (REGEX REPLACE
                                "/" "\\\\"
                                NATIVE_BIN_PATH_FILES ${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_NATIVE_BIN_PATH_FILES})

                        set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}\
            nestedBin: {\n\
                #output {\n\
                    package = redist;\n\
                };\n\
                #destination = \${d_bin}/${${_name}_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}_BIN_PATH_PREFIX};\n\
                #add-each-file = ::nuget.[redist]targets.[${condition}].CopyToOutput;\n\
                ${NATIVE_BIN_PATH_FILES}\n\
            };\n")
                    endif ()

                    set (NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION} "${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}\
        }\n\
\n")

                    set (NUSPEC_CONFIGURATIONS "${NUSPEC_CONFIGURATIONS}${NUSPEC_${NUSPEC_PLATFORM}_${NUSPEC_TOOLSET}_${NUSPEC_CONFIGURATION}}")
                endforeach ()
            endforeach ()
        endforeach ()
    endforeach ()

    set (NUSPEC_DEFINES "")
    foreach (DEFINE ${${_name}_DEFINES})
        set (NUSPEC_DEFINES "${NUSPEC_DEFINES}\
        Defines += ${DEFINE};\n")
    endforeach ()

    foreach (NUSPEC_CONFIGURATION ${${_name}_CONFIGURATIONS})
        if (${_name}_${NUSPEC_CONFIGURATION}_DEFINES)
            set (NUSPEC_DEFINES "${NUSPEC_DEFINES}\
        [${NUSPEC_CONFIGURATION}]\n")
            foreach (DEFINE ${${_name}_${NUSPEC_CONFIGURATION}_DEFINES})
                set (NUSPEC_DEFINES "${NUSPEC_DEFINES}\
            Defines += ${DEFINE};\n")
            endforeach ()
        endif ()
    endforeach ()

    set (NUSPEC_TARGETS "\
    targets {\n\
${NUSPEC_DEFINES}\
    }\n\
\n")

    set (NUSPEC_FILES "\
    files {\n\
${NUSPEC_INCLUDE}\
${NUSPEC_CONFIGURATIONS}\
    };\n")

    set (${_nuspec} "\
nuget {\n\
${NUSPEC_HEADER}\n\
${NUSPEC_DEPENDENCIES}\n\
${NUSPEC_FILES}\n\
${NUSPEC_TARGETS}\n\
}" PARENT_SCOPE)

endfunction (generate_nuspec_autopkg)

function (download_and_extract_7z _url _dest)
    find_program(7ZIP_EXECUTABLE NAMES 7z.exe
            HINTS PATHS "c:/Program Files/7-Zip")

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(7Zip DEFAULT_MSG 7ZIP_EXECUTABLE)

    mark_as_advanced(7ZIP_EXECUTABLE)

    get_filename_component(_file ${_url} NAME)
    message (${_url})
    file (DOWNLOAD ${_url} ${_dest}/${_file})
    execute_process(COMMAND ${7ZIP_EXECUTABLE} -y x ${_dest}/${_file} WORKING_DIRECTORY ${_dest})
    if (${_file} MATCHES .tar)
        string(REPLACE ".7z" "" tarball ${_file})
        string(REPLACE ".lzma" "" tarball ${_file})
        execute_process(COMMAND ${7ZIP_EXECUTABLE} -y x ${_dest}/${tarball} WORKING_DIRECTORY ${_dest})
    endif ()
endfunction (download_and_extract_7z)

macro (import_nuget_packages target)
    foreach (dep ${${target}_PACKAGES})
        string (REPLACE "/" ";" dep ${dep})
        list (GET dep 0 package)
        list (GET dep 1 version)
        message ("PACKAGE: ${package} - VERSION: ${version}")

        if (TARGET ${package})
            add_dependencies(${target} ${package})
            get_target_property(${package}_INCLUDE_DIR ${package} INCLUDE_DIRECTORIES)
            include_directories (SYSTEM ${${package}_INCLUDE_DIR})
            get_target_property(${package}_TYPE ${package} TYPE)
            if (NOT ${${package}_TYPE} STREQUAL EXECUTABLE)
                target_link_libraries (${target} ${package})
            endif ()
            add_custom_command (TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E
                    copy $<TARGET_FILE:${package}> $<TARGET_FILE_DIR:${target}>)
        else ()
            execute_process(COMMAND ${ROOT_SOURCE_DIR}/CMake/tools/.nuget/NuGet.exe install ${package} -Version ${version} -SolutionDirectory ${ROOT_BINARY_DIR} -ConfigFile ${ROOT_SOURCE_DIR}/CMake/tools/.nuget/NuGet.config)
        endif ()
    endforeach ()
    if (${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${ROOT_BINARY_DIR})
        set(PACKAGE_LOCATION ".")
    else ()
        file (RELATIVE_PATH PACKAGE_LOCATION ${CMAKE_CURRENT_BINARY_DIR} ${ROOT_BINARY_DIR})
    endif ()
    string (REGEX REPLACE "/" "\\\\" PACKAGE_LOCATION ${PACKAGE_LOCATION})
    execute_process(COMMAND powershell -NoProfile -ExecutionPolicy Bypass -Command "& {Import-Module '${ROOT_SOURCE_DIR}/CMake/powershell/NuGetImportUtils' -Force; set-location '${CMAKE_CURRENT_BINARY_DIR}'; Import-NuGetPackages ${target} ${PACKAGE_LOCATION}} >$null 2>&1")
endmacro (import_nuget_packages)
