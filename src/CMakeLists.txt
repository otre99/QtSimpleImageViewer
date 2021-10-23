cmake_minimum_required(VERSION 3.5)

project(ImageViewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    asyncimageloader.cpp
    imageslistmodel.cpp
    imageviewer.cpp
    mainwindow.ui
    resources.qrc
    cropwidget.cpp
    image_utils/image_utils.cpp
    image_utils/autocropborder.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX} )
