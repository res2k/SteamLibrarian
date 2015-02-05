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

add_library(QmlSteamLibrarian SHARED
            plugin/SteamLibrarian.hpp
            plugin/SteamLibrarian.cpp
           )
target_link_libraries(QmlSteamLibrarian
	              Piping
		      Qt5::Qml)

install(TARGETS QmlSteamLibrarian
        RUNTIME
        DESTINATION ${CMAKE_BINARY_DIR}/qml/SteamLibrarian
        CONFIGURATIONS Release RelWithDebInfo)
install(FILES plugin/qmldir
        DESTINATION ${CMAKE_BINARY_DIR}/qml/SteamLibrarian
        CONFIGURATIONS Release RelWithDebInfo)
install(CODE "execute_process(COMMAND \"${_qt5Core_install_prefix}/bin/qmlplugindump.exe\" SteamLibrarian 1.0 \"${CMAKE_BINARY_DIR}/qml\" OUTPUT_FILE \"${CMAKE_BINARY_DIR}/qml/SteamLibrarian/plugins.qmltypes\")")
