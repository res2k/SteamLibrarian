cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
PROJECT(SteamLibrarian)

find_package(Qt5 REQUIRED COMPONENTS Qml Widgets)
set(CMAKE_AUTOMOC ON)

add_subdirectory(piping)

qt5_add_resources(QML_QRC qml.qrc OPTIONS --compress 9 --threshold 0)
add_executable(SteamLibrarian main.cpp main.qml ${QML_QRC})
target_link_libraries(SteamLibrarian
	              Piping
		      Qt5::Qml Qt5::Widgets)
