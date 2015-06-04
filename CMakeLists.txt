cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
PROJECT(SteamLibrarian)

# Make sure version info is regenerated if the repo changes
set_directory_properties(PROPERTIES CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/.git/index")

include(build/version.cmake)
# Create a header with the version definitions
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/version)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version/version.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/version)

find_package(Qt5 REQUIRED COMPONENTS Concurrent Qml Quick Widgets WinExtras)
set(CMAKE_AUTOMOC ON)
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.57 REQUIRED filesystem iostreams system)
add_definitions(-DBOOST_ALL_NO_LIB)

find_package(CxxTest)

if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
endif()

add_subdirectory(facade)
add_subdirectory(piping)

add_executable(SteamLibrarian main.cpp resources/SteamLibrarian.rc)
target_link_libraries(SteamLibrarian
	              Facade Piping
		      Qt5::Qml Qt5::Widgets Qt5::WinMain
                      uxtheme.lib)
SET_TARGET_PROPERTIES(SteamLibrarian PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS"
                                                LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
                                                LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

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
        CONFIGURATIONS Release RelWithDebInfo
        COMPONENT plugin)
install(FILES plugin/qmldir
        DESTINATION ${CMAKE_BINARY_DIR}/qml/SteamLibrarian
        CONFIGURATIONS Release RelWithDebInfo
        COMPONENT plugin)
install(CODE "execute_process(COMMAND \"${_qt5Core_install_prefix}/bin/qmlplugindump.exe\" SteamLibrarian 1.0 \"${CMAKE_BINARY_DIR}/qml\" OUTPUT_FILE \"${CMAKE_BINARY_DIR}/qml/SteamLibrarian/plugins.qmltypes\")"
        COMPONENT plugin)

add_subdirectory(installer)
