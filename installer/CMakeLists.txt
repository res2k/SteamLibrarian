CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/installer.sln.in" "${CMAKE_CURRENT_BINARY_DIR}/installer.sln")

SET(INSTALLER_OUTPUT "SteamLibrarian.exe")
ADD_CUSTOM_COMMAND(OUTPUT ${INSTALLER_OUTPUT}
                   COMMAND msbuild installer.sln "/p:Configuration=Release;StmLbnVersion=${STMLBN_VERSION_FULL};CMakeConfig=$<CONFIG>"
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS SteamLibrarian
                   COMMENT "Building installer package")
ADD_CUSTOM_TARGET(BUILD_INSTALLER ALL
                  SOURCES ${INSTALLER_OUTPUT}
                          definitions.wxi
                          MsiPackage.wxs 
                          ExePackage.wxs
                          collect_libs.cmake)
IF(CMAKE_GENERATOR_PLATFORM)
  SET(CRT_PLATFORM ${CMAKE_GENERATOR_PLATFORM})
ELSE()
  SET(CRT_PLATFORM "x86")
ENDIF()

STRING(REGEX REPLACE "[^0-9]" "" VC_VERSION "${CMAKE_VS_PLATFORM_TOOLSET}")

SET(VC_INSTALLDIR "${CMAKE_CXX_COMPILER}")
WHILE(VC_INSTALLDIR)
  GET_FILENAME_COMPONENT(VC_INSTALLDIR "${VC_INSTALLDIR}" DIRECTORY)
  IF(EXISTS "${VC_INSTALLDIR}/redist")
    BREAK()
  ENDIF()
ENDWHILE()
IF(NOT VC_INSTALLDIR)
  MESSAGE(FATAL_ERROR "Couldn't determine VC install directory!")
ENDIF()

ADD_CUSTOM_COMMAND(TARGET BUILD_INSTALLER
                   PRE_BUILD
                   COMMAND "${CMAKE_COMMAND}"
                      -DOUTPUT_DIR=deps-$<CONFIG>
                      -DCRT_PLATFORM=${CRT_PLATFORM}
                      -DVC_VERSION=${VC_VERSION}
                      "-DVC_INSTALLDIR=${VC_INSTALLDIR}/"
                      "-DMAIN_BINARY=$<TARGET_FILE:SteamLibrarian>"
                      "-DQT_PREFIX=${_qt5Core_install_prefix}"
                      -P "${CMAKE_CURRENT_SOURCE_DIR}/collect_libs.cmake")
