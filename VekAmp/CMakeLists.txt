cmake_minimum_required(VERSION 3.16)

project(VekAmp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appVekAmp
    main.cpp
    bassplayer.cpp
    utils.cpp
)

qt_add_qml_module(appVekAmp
    URI VekAmp
    QML_FILES
        Main.qml
    SOURCES
        bassuibackend.hpp
        bassuibackend.cpp
    RESOURCES
        resources.qrc
)


# BASS linkage
set(BASS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/BASS)
set(BASS_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/BASS/Lib)

target_include_directories(appVekAmp PUBLIC
    ${BASS_DIR}
)

add_library(BASS SHARED IMPORTED)
set_target_properties(BASS PROPERTIES
    IMPORTED_LOCATION ${BASS_LIB_DIR}/bass.dll
    IMPORTED_IMPLIB ${BASS_LIB_DIR}/bass.lib
)

add_library(BASSALAC SHARED IMPORTED)
set_target_properties(BASSALAC PROPERTIES
    IMPORTED_LOCATION ${BASS_LIB_DIR}/bassalac.dll
    IMPORTED_IMPLIB ${BASS_LIB_DIR}/bassalac.lib
)

add_library(BASSAPE SHARED IMPORTED)
set_target_properties(BASSAPE PROPERTIES
    IMPORTED_LOCATION ${BASS_LIB_DIR}/bassape.dll
    IMPORTED_IMPLIB ${BASS_LIB_DIR}/bassape.lib
)

add_library(BASSFLAC SHARED IMPORTED)
set_target_properties(BASSFLAC PROPERTIES
    IMPORTED_LOCATION ${BASS_LIB_DIR}/bassflac.dll
    IMPORTED_IMPLIB ${BASS_LIB_DIR}/bassflac.lib
)

add_library(BASSOPUS SHARED IMPORTED)
set_target_properties(BASSOPUS PROPERTIES
    IMPORTED_LOCATION ${BASS_LIB_DIR}/bassopus.dll
    IMPORTED_IMPLIB ${BASS_LIB_DIR}/bassopus.lib
)

set(BASS_TARGETS
    BASS;
    BASSALAC;
    BASSAPE;
    BASSFLAC;
    BASSOPUS;
)

target_link_libraries(appVekAmp PUBLIC
    ${BASS_TARGETS}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appVekAmp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appVekAmp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appVekAmp
    PRIVATE Qt6::Quick
)
target_link_libraries(appVekAmp PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appVekAmp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
