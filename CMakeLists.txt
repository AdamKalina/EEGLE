cmake_minimum_required(VERSION 3.5)

project(EEGLE LANGUAGES C CXX) # here must be C CXX because of fidlib.c

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    read_signal_file.cpp
    show_hdr.cpp
    viewcurve.cpp
    #third_party/fidlib/fidlib.c
    resources.qrc
    )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EEGle
        ${PROJECT_SOURCES}
        )
else()
    if(ANDROID)
        add_library(EEGle SHARED
            ${PROJECT_SOURCES}
            )
    else()
        add_executable(EEGle
            ${PROJECT_SOURCES}
            )
    endif()
endif()

target_link_libraries(EEGle PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
