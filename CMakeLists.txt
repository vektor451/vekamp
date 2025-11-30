cmake_minimum_required(VERSION 3.21.0)
project(VekAmp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if(WIN32)
    set(EXE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
    set(LIB_PATH /libs/win64)
elseif(UNIX)
    set(EXE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(LIB_PATH /libs/linux-x86_64)
endif()

find_package(PkgConfig REQUIRED)

# QT --------------------------------------------------------------------------
message("==   Init QT   ==")
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

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

if(WIN32)
    set(BASS_DLL_PATH       ${BASS_DIR}${LIB_PATH}/bass.dll)
    set(BASSALAC_DLL_PATH   ${BASSALAC_DIR}${LIB_PATH}/bassalac.dll)
    set(BASSAPE_DLL_PATH    ${BASSAPE_DIR}${LIB_PATH}/bassape.dll)
    set(BASSFLAC_DLL_PATH   ${BASSFLAC_DIR}${LIB_PATH}/bassflac.dll)
    set(BASSOPUS_DLL_PATH   ${BASSOPUS_DIR}${LIB_PATH}/bassopus.dll)

    set(BASS_LIB_PATH       ${BASS_DIR}${LIB_PATH}/bass.lib)
    set(BASSALAC_LIB_PATH   ${BASSALAC_DIR}${LIB_PATH}/bassalac.lib)
    set(BASSAPE_LIB_PATH    ${BASSAPE_DIR}${LIB_PATH}/bassape.lib)
    set(BASSFLAC_LIB_PATH   ${BASSFLAC_DIR}${LIB_PATH}/bassflac.lib)
    set(BASSOPUS_LIB_PATH   ${BASSOPUS_DIR}${LIB_PATH}/bassopus.lib)
    
    set(BASS_DIR_PATHS
        ${BASS_DIR_PATH};
        ${BASSALAC_DIR_PATH};
        ${BASSAPE_DIR_PATH};
        ${BASSFLAC_DIR_PATH};
        ${BASSOPUS_DIR_PATH};
    )
    
    set(BASS_LIB_PATHS
        ${BASS_LIB_PATH};
        ${BASSALAC_LIB_PATH};
        ${BASSAPE_LIB_PATH};
        ${BASSFLAC_LIB_PATH};
        ${BASSOPUS_LIB_PATH};
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
 
add_library(BASS SHARED IMPORTED)
set_target_properties(BASS PROPERTIES
    IMPORTED_LOCATION ${BASS_DLL_PATH}
    IMPORTED_IMPLIB ${BASS_LIB_PATH}
    Qt6::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE"
    Qt6::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG"
)

add_library(BASSALAC SHARED IMPORTED) 
set_target_properties(BASSALAC PROPERTIES
    IMPORTED_LOCATION ${BASSALAC_DLL_PATH}
    IMPORTED_IMPLIB ${BASSALAC_LIB_PATH}
)

add_library(BASSAPE SHARED IMPORTED) 
set_target_properties(BASSAPE PROPERTIES
    IMPORTED_LOCATION ${BASSAPE_DLL_PATH}
    IMPORTED_IMPLIB ${BASSAPE_LIB_PATH}
)

add_library(BASSFLAC SHARED IMPORTED) 
set_target_properties(BASSFLAC PROPERTIES
    IMPORTED_LOCATION ${BASSFLAC_DLL_PATH}
    IMPORTED_IMPLIB ${BASSFLAC_LIB_PATH}
)

add_library(BASSOPUS SHARED IMPORTED) 
set_target_properties(BASSOPUS PROPERTIES
    IMPORTED_LOCATION ${BASSOPUS_DLL_PATH}
    IMPORTED_IMPLIB ${BASSOPUS_LIB_PATH}
)

set(BASS_TARGETS
    BASS;
    BASSALAC;
    BASSAPE;
    BASSFLAC;
    BASSOPUS;
)

# TagLib ----------------------------------------------------------------------
#message("== Init TagLib ==")
#set(USER_HOME_DIR $ENV{USERPROFILE})

#find_package(ZLIB)
#message("Looking in dir: ${USER_HOME_DIR}\\projects\\taglib\\src\\msvs_vcpkg_build")
#set(CMAKE_PREFIX_PATH ${USER_HOME_DIR}\\projects\\taglib\\src\\msvs_vcpkg_build)
#pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib)

# MAIN ------------------------------------------------------------------------
message ("==    MAIN    ==")

qt_add_executable(${PROJECT_NAME}
    src/utils.cpp
    src/bassplayer.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.ui
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Link BASS
target_link_libraries(${PROJECT_NAME} PUBLIC ${BASS_TARGETS}) 
target_include_directories(${PROJECT_NAME} PRIVATE ${BASS_INCLUDE_DIRS})

# Link QT6
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

# Link TagLib
#target_link_libraries(${PROJECT_NAME} TagLib::tag)
#target_link_libraries(${PROJECT_NAME} TagLib::tag_c)

#target_sources(${PROJECT_NAME} PRIVATE
#    src/utils.cpp
#    src/bassplayer.cpp
#    src/main.cpp
#    src/mainwindow.cpp
#)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

#Post build commands
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy ${BASS_DLL_PATH} ${EXE_PATH};
#    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSALAC_DLL_PATH} ${EXE_PATH};
#    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSAPE_DLL_PATH} ${EXE_PATH};
#    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSFLAC_DLL_PATH} ${EXE_PATH};
#    COMMAND "${CMAKE_COMMAND}" -E copy ${BASSOPUS_DLL_PATH} ${EXE_PATH};
#
#    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${EXE_PATH}/assets
#    VERBATIM
#)