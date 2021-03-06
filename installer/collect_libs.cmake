SET(OUTPUT_DIR_FULL "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_DIR}")
FILE(MAKE_DIRECTORY "${OUTPUT_DIR_FULL}")
STRING(UUID TEMP_DIR_UUID NAMESPACE 9E594149-F7F2-462C-A11C-DBA4D3577937 NAME "${OUTPUT_DIR}" TYPE SHA1)

# Copy MSVC runtime DLLs to dependencies directory
FUNCTION(COLLECT_MSVCRT)
  SET(CRT_LIBS_DIR "${VC_INSTALLDIR}redist/${CRT_PLATFORM}/Microsoft.VC${VC_VERSION}.CRT")
  MESSAGE(STATUS "CRT libs directory: ${CRT_LIBS_DIR}")
  
  FILE(GLOB CRT_LIBS "${CRT_LIBS_DIR}/msvc*.dll")
  FILE(COPY ${CRT_LIBS} DESTINATION "${OUTPUT_DIR_FULL}")
ENDFUNCTION()

# Obtain dependent libs for a binary
FUNCTION(FETCH_DEPENDENT_LIBS DEPS_LIST BINARY_PATH)
  GET_FILENAME_COMPONENT(BINARY_NAME "${BINARY_PATH}" NAME)
  SET(DUMPBIN_TEMP_FILE "${TEMP_DIR}/${BINARY_NAME}.txt")
  EXECUTE_PROCESS(COMMAND dumpbin /NOLOGO /DEPENDENTS "${BINARY_PATH}"
                  OUTPUT_VARIABLE DUMPBIN_LINES)
  STRING(REGEX REPLACE ";" "\\\\;" DUMPBIN_LINES "${DUMPBIN_LINES}")
  STRING(REGEX REPLACE "\n" ";" DUMPBIN_LINES "${DUMPBIN_LINES}")
  FOREACH(LINE ${DUMPBIN_LINES})
    STRING(REGEX MATCH "^ +(.*\\.dll)$" LINE_MATCH ${LINE})
    IF(LINE_MATCH)
      LIST(APPEND DEP_LINES ${CMAKE_MATCH_1})
    ENDIF()
  ENDFOREACH()
  SET(${DEPS_LIST} ${DEP_LINES} PARENT_SCOPE)
ENDFUNCTION()

# Recursively collect dependent libs
FUNCTION(COLLECT_DEPS_RECURSIVE ALL_DEPS LIBS_DIR)
  SET(BINARIES_TO_CHECK ${ARGN})
  WHILE(BINARIES_TO_CHECK)
    LIST(GET BINARIES_TO_CHECK 0 BINARY)
    LIST(REMOVE_AT BINARIES_TO_CHECK 0)
    GET_FILENAME_COMPONENT(BINARY_NAME "${BINARY}" NAME)
    LIST(FIND CHECKED_BINARIES "${BINARY_NAME}" FOUND_INDEX)
    IF(NOT FOUND_INDEX EQUAL -1)
      continue()
    ENDIF()
    FETCH_DEPENDENT_LIBS(BINARY_DEPS "${BINARY}")
    FOREACH(DEP_BINARY ${BINARY_DEPS})
      SET(FULL_BINARY_PATH "${LIBS_DIR}/${DEP_BINARY}")
      IF(NOT EXISTS ${FULL_BINARY_PATH})
        continue()
      ENDIF()
      LIST(APPEND BINARIES_TO_CHECK "${FULL_BINARY_PATH}")
      LIST(APPEND FOUND_BINARIES "${FULL_BINARY_PATH}")
    ENDFOREACH()
    LIST(APPEND CHECKED_BINARIES "${BINARY_NAME}")
  ENDWHILE()
  LIST(REMOVE_DUPLICATES FOUND_BINARIES)
  SET(${ALL_DEPS} ${FOUND_BINARIES} PARENT_SCOPE)
ENDFUNCTION()

# Qt plugins we need
SET(QT_PLUGINS
    "qml/QtQuick/Controls/qtquickcontrolsplugin.dll"
    "qml/QtQuick/Layouts/qquicklayoutsplugin.dll"
    "qml/QtQuick/Window.2/windowplugin.dll"
    "qml/QtQuick.2/qtquick2plugin.dll"
    "plugins/imageformats/qsvg.dll"
    "plugins/platforms/qwindows.dll")
FOREACH(PLUGIN ${QT_PLUGINS})
  SET(QT_PLUGIN_FULL "${QT_PREFIX}/${PLUGIN}")
  LIST(APPEND QT_PLUGINS_FULL ${QT_PLUGIN_FULL})
  GET_FILENAME_COMPONENT(DEST_DIR "${OUTPUT_DIR_FULL}/${PLUGIN}" DIRECTORY)
  FILE(COPY "${QT_PLUGIN_FULL}" DESTINATION "${DEST_DIR}")
  GET_FILENAME_COMPONENT(SRC_DIR "${QT_PLUGIN_FULL}" DIRECTORY)
  IF(EXISTS "${SRC_DIR}/qmldir")
    FILE(COPY "${SRC_DIR}/qmldir" DESTINATION "${DEST_DIR}")
  ENDIF()
ENDFOREACH()

# EGL, GLES
SET(EXTRA_BIN
    "libEGL.dll"
    "libGLESv2.dll"
    "d3dcompiler_47.dll")
FOREACH(EXTRA ${EXTRA_BIN})
  SET(EXTRA_FULL "${QT_PREFIX}/bin/${EXTRA}")
  LIST(APPEND EXTRA_BINS_FULL ${EXTRA_FULL})
  GET_FILENAME_COMPONENT(DEST_DIR "${OUTPUT_DIR_FULL}/${EXTRA}" DIRECTORY)
  FILE(COPY "${EXTRA_FULL}" DESTINATION "${DEST_DIR}")
ENDFOREACH()

COLLECT_MSVCRT()
COLLECT_DEPS_RECURSIVE(ALL_DEPS "${QT_PREFIX}/bin" "${MAIN_BINARY}" ${QT_PLUGINS_FULL} ${EXTRA_BINS_FULL})
FILE(COPY ${ALL_DEPS} DESTINATION "${OUTPUT_DIR_FULL}")

