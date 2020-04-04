cmake_minimum_required(VERSION 3.12.0)

project(Group32ModelViewer)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	FILE(COPY Resources/splashscreen.png DESTINATION "${CMAKE_BINARY_DIR}/Debug")
	FILE(COPY Resources/openIcon.png DESTINATION "${CMAKE_BINARY_DIR}/Debug")
	FILE(COPY Resources/screenshotIcon.png DESTINATION "${CMAKE_BINARY_DIR}/Debug")
else ()
	FILE(COPY Resources/splashscreen.png DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY Resources/openIcon.png DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY Resources/screenshotIcon.png DESTINATION "${CMAKE_BINARY_DIR}")
endif ()
