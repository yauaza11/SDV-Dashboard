cmake_minimum_required(VERSION 3.19)
project(sdv_control LANGUAGES CXX)
set(CMAKE_AUTORCC ON)
set(OpenCV_DIR "C:/msys64/mingw64/lib/cmake/opencv4")
# set(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/mingw_64")
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network QuickWidgets Charts)
# find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
find_package(OpenCV REQUIRED)


qt_standard_project_setup()

qt_add_executable(sdv_control
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
)

target_link_libraries(sdv_control
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Widgets
        Qt6::Network
        Qt6::QuickWidgets
        Qt6::Charts
        ${OpenCV_LIBS}
)

include(GNUInstallDirs)
include_directories(${OpenCV_INCLUDE_DIRS})

install(TARGETS sdv_control
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET sdv_control
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
