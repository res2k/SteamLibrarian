cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
PROJECT(SteamLibrarian)

find_package(Qt5 REQUIRED COMPONENTS Concurrent Qml Widgets)
set(CMAKE_AUTOMOC ON)

add_subdirectory(facade)
add_subdirectory(piping)

add_executable(SteamLibrarian main.cpp)
target_link_libraries(SteamLibrarian
	              Facade Piping
		      Qt5::Qml Qt5::Widgets)
