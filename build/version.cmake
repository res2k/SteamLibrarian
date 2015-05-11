include(${CMAKE_SOURCE_DIR}/build/version-num.cmake)

find_package(Git)

# Obtain the last commit for this file
EXECUTE_PROCESS(COMMAND "${GIT_EXECUTABLE}" log -1 --pretty=format:%h build/version-num.cmake
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				OUTPUT_VARIABLE _MY_COMMIT
				OUTPUT_STRIP_TRAILING_WHITESPACE)
# Compute the number of changes since the last change to the version number
EXECUTE_PROCESS(COMMAND "${GIT_EXECUTABLE}" rev-list ^${_MY_COMMIT} HEAD --count
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				OUTPUT_VARIABLE STMLBN_VERSION_REV
				OUTPUT_STRIP_TRAILING_WHITESPACE)
