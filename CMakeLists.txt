cmake_minimum_required(VERSION 3.16)

project(FunctionPlotter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Qml)


set(SOURCES main.cpp PlotterApp.cpp)
set(HEADERS PlotterApp.h)

add_executable(FunctionPlotter ${SOURCES} ${HEADERS})

target_link_libraries(FunctionPlotter PRIVATE
    Qt6::Widgets
    Qt6::Charts
    Qt6::Qml  # Added for QJSEngine
)

set_target_properties(FunctionPlotter PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS FunctionPlotter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(FunctionPlotter)
