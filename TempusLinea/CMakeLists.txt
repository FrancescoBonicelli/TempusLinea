cmake_minimum_required(VERSION 3.5)

project(TempusLinea VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        conf.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        canvas.cpp
        canvas.h
        era.cpp
        era.h
        eraForm.cpp
        eraForm.h
        mouseMenu.cpp
        mouseMenu.h
        colorPicker.cpp
        colorPicker.h
        leftMenu.cpp
        leftMenu.h
        category.cpp
        category.h
        event.cpp
        event.h
        period.cpp
        period.h
        categoriesManager.cpp
        categoriesManager.h
        categoryForm.cpp
        categoryForm.h
        eventForm.cpp
        eventForm.h
        periodForm.cpp
        periodForm.h
        eventWidget.cpp
        eventWidget.h

        resources.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TempusLinea
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TempusLinea APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TempusLinea SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TempusLinea
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TempusLinea PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(TempusLinea PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

configure_file(TempusLineaLogo_100.png TempusLineaLogo_100.png COPYONLY)
configure_file(eye-on.png eye-on.png COPYONLY)
configure_file(eye-off.png eye-off.png COPYONLY)
configure_file(stylesheet.qss stylesheet.qss COPYONLY)
configure_file(TempusLinea.ico TempusLinea.ico COPYONLY)

install(TARGETS TempusLinea
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TempusLinea)
endif()
