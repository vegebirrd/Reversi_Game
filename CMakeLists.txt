cmake_minimum_required(VERSION 3.5)

project(Reversi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Reversi
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        boardwidget.h boardwidget.cpp
        globals.h
        resources.qrc

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Reversi APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Reversi SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Reversi
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Reversi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Reversi)
endif()
set_target_properties(Reversi PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Reversi
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Qt6 COMPONENTS Multimedia REQUIRED)
target_link_libraries(Reversi PRIVATE Qt6::Multimedia)

# 指定资源文件的安装路径
set(RESOURCE_FILES
    bg.jpg
    mute.png
    No Surprises.flac
    PKU.JPG
    unmute.jpeg
    Peking.icns
)

# 为资源文件创建一个循环，将它们安装到 .app 包的 Resources 目录中
foreach(RESOURCE ${RESOURCE_FILES})
    set_source_files_properties(${RESOURCE} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)
endforeach()

# 设置应用程序的图标文件
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE Peking.icns)
    # 确保.icns文件在正确的位置
    set(APP_ICON_MACOSX /Users/zhangwenhao/Reversi/Peking.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)
endif()


# 添加一个安装目标，这将在构建后将文件复制到正确的位置
install(FILES ${RESOURCE_FILES}
        DESTINATION ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Reversi)
endif()
