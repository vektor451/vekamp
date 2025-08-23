cmake_minimum_required(VERSION 3.13.0)
project(vekamp)

#set output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if(WIN32)
    set(EXE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}debug)
    set(LIB_PATH /libs/win64)
elseif(UNIX)
    set(EXE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(LIB_PATH /libs/linux-x86_64)
endif()

find_package(PkgConfig REQUIRED)

# BASS ------------------------------------------------------------------------
set(BASS_DIR       ${VENDOR_DIR}/bass24)
set(BASSALAC_DIR   ${VENDOR_DIR}/bassalac24)
set(BASSAPE_DIR    ${VENDOR_DIR}/bassape24)
set(BASSFLAC_DIR   ${VENDOR_DIR}/bassflac24)
set(BASSOPUS_DIR   ${VENDOR_DIR}/bassopus24)

set(BASS_INCLUDE_DIRS 
    ${BASS_DIR};
    ${BASSALAC_DIR};
    ${BASSAPE_DIR};
    ${BASSFLAC_DIR};
    ${BASSOPUS_DIR};
)

message("==  Init BASS  ==")
message("Looking in dir: ${BASS_DIR}${LIB_PATH}")

add_library(BASS        SHARED IMPORTED) 
add_library(BASSALAC    SHARED IMPORTED) 
add_library(BASSAPE     SHARED IMPORTED) 
add_library(BASSFLAC    SHARED IMPORTED) 
add_library(BASSOPUS    SHARED IMPORTED) 

if(WIN32)
    set(BASS_DLL_PATH       ${BASS_DIR}${LIB_PATH}/bass.dll)
    set(BASSALAC_DLL_PATH   ${BASSALAC_DIR}${LIB_PATH}/bassalac.dll)
    set(BASSAPE_DLL_PATH    ${BASSAPE_DIR}${LIB_PATH}/bassape.dll)
    set(BASSFLAC_DLL_PATH   ${BASSFLAC_DIR}${LIB_PATH}/bassflac.dll)
    set(BASSOPUS_DLL_PATH   ${BASSOPUS_DIR}${LIB_PATH}/bassopus.dll)
    
    set(BASS_LIB_PATHS
        ${BASS_DIR}${LIB_PATH}/bass.lib;
        ${BASSALAC_DIR}${LIB_PATH}/bassalac.lib;
        ${BASSAPE_DIR}${LIB_PATH}/bassape.lib;
        ${BASSFLAC_DIR}${LIB_PATH}/bassflac.lib;
        ${BASSOPUS_DIR}${LIB_PATH}/bassopus.lib;
    )
elseif (UNIX)
    set(BASS_DLL_PATH       ${BASS_DIR}${LIB_PATH}/libbass.so)
    set(BASSALAC_DLL_PATH   ${BASSALAC_DIR}${LIB_PATH}/bassalac.so)
    set(BASSAPE_DLL_PATH    ${BASSAPE_DIR}${LIB_PATH}/bassape.so)
    set(BASSFLAC_DLL_PATH   ${BASSFLAC_DIR}${LIB_PATH}/bassflac.so)
    set(BASSOPUS_DLL_PATH   ${BASSOPUS_DIR}${LIB_PATH}/bassopus.so)
    
    set(BASS_LIB_PATHS
        ${BASS_DLL_PATH};
        ${BASSALAC_DLL_PATH};
        ${BASSAPE_DLL_PATH};
        ${BASSFLAC_DLL_PATH};
        ${BASSOPUS_DLL_PATH};
    )    
 endif()

# TagLib ----------------------------------------------------------------------
#message("== Init TagLib ==")
#set(USER_HOME_DIR $ENV{USERPROFILE})

#find_package(ZLIB)
#message("Looking in dir: ${USER_HOME_DIR}\\projects\\taglib\\src\\msvs_vcpkg_build")
#set(CMAKE_PREFIX_PATH ${USER_HOME_DIR}\\projects\\taglib\\src\\msvs_vcpkg_build)
#pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib)

# GTK4 ------------------------------------------------------------------------
set(GTK_WIN_PATH C:/gtk-build/gtk/x64/release)

message("==  Init GTK4  ==")

if(WIN32)
    set(CMAKE_PREFIX_PATH ${GTK_WIN_PATH})
    message("Looking in dir: ${GTK_WIN_PATH}")
endif()

# Use the package PkgConfig to detect GTK+ headers/library files
pkg_check_modules(GTK4 REQUIRED gtk4)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
set(CMAKE_SYSTEM_PREFIX_PATH ${GTK4_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK4_CFLAGS_OTHER})

# MAIN ------------------------------------------------------------------------
message ("==    MAIN    ==")

add_executable(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Link GTK4
target_link_libraries(${PROJECT_NAME} ${GTK4_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GTK4_INCLUDE_DIRS})
# Link BASS
target_link_libraries(${PROJECT_NAME} ${BASS_LIB_PATHS}) 
target_include_directories(${PROJECT_NAME} PRIVATE ${BASS_INCLUDE_DIRS})
# Link TagLib
#target_link_libraries(${PROJECT_NAME} TagLib::tag)
#target_link_libraries(${PROJECT_NAME} TagLib::tag_c)

target_sources(${PROJECT_NAME} PRIVATE
    src/utils.cpp
    src/bassplayer.cpp
    src/main.cpp
)

# Post build commands
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy ${BASS_DLL_PATH} ${EXE_PATH};
    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSALAC_DLL_PATH} ${EXE_PATH};
    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSAPE_DLL_PATH} ${EXE_PATH};
    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSFLAC_DLL_PATH} ${EXE_PATH};
    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSOPUS_DLL_PATH} ${EXE_PATH};

    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${EXE_PATH}/assets
    VERBATIM
)