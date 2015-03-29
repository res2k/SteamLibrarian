cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
PROJECT(SteamLibrarian)

find_package(Qt5 REQUIRED COMPONENTS Concurrent Qml Widgets WinExtras)
set(CMAKE_AUTOMOC ON)
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.57 REQUIRED filesystem iostreams system)
add_definitions(-DBOOST_ALL_NO_LIB)

add_subdirectory(facade)
add_subdirectory(piping)

add_executable(SteamLibrarian main.cpp)
target_link_libraries(SteamLibrarian
	              Facade Piping
		      Qt5::Qml Qt5::Widgets
                      uxtheme.lib)

add_library(QmlSteamLibrarian SHARED
            plugin/SteamLibrarian.hpp
            plugin/SteamLibrarian.cpp
           )
target_link_libraries(QmlSteamLibrarian
	              Facade Piping
		      Qt5::Qml)

install(TARGETS QmlSteamLibrarian
        RUNTIME
        DESTINATION ${CMAKE_BINARY_DIR}/qml/SteamLibrarian
        CONFIGURATIONS Release RelWithDebInfo)
install(FILES plugin/qmldir
        DESTINATION ${CMAKE_BINARY_DIR}/qml/SteamLibrarian
        CONFIGURATIONS Release RelWithDebInfo)
install(CODE "execute_process(COMMAND \"${_qt5Core_install_prefix}/bin/qmlplugindump.exe\" SteamLibrarian 1.0 \"${CMAKE_BINARY_DIR}/qml\" OUTPUT_FILE \"${CMAKE_BINARY_DIR}/qml/SteamLibrarian/plugins.qmltypes\")")
