#This CMakeLists ensures that any assets required by the application are copied to the location of the binaries so that they can be accessed at runtime

cmake_minimum_required(VERSION 3.12.0)

project(Group32ModelViewer)

#MSVC puts binaries inside Debug/Release subdirectory whilst other known compilers do not. The following conditional checks the compiler being used and ensures that the necessary resources are copied to the correct location.
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		FILE(COPY Resources/splashscreen.png DESTINATION "${CMAKE_BINARY_DIR}/Release")
		FILE(COPY Resources/openIcon.png DESTINATION "${CMAKE_BINARY_DIR}/Release")
		FILE(COPY Resources/screenshotIcon.png DESTINATION "${CMAKE_BINARY_DIR}/Release")
		FILE(COPY Resources/Group32ModelViewerLogo.ico DESTINATION "${CMAKE_BINARY_DIR}/Release")
		FILE(COPY Resources/logo.stl DESTINATION "${CMAKE_BINARY_DIR}/Release")
	else()
		FILE(COPY Resources/splashscreen.png DESTINATION "${CMAKE_BINARY_DIR}/Debug")
		FILE(COPY Resources/openIcon.png DESTINATION "${CMAKE_BINARY_DIR}/Debug")
		FILE(COPY Resources/screenshotIcon.png DESTINATION "${CMAKE_BINARY_DIR}/Debug")
		FILE(COPY Resources/Group32ModelViewerLogo.ico DESTINATION "${CMAKE_BINARY_DIR}/Debug")
		FILE(COPY Resources/logo.stl DESTINATION "${CMAKE_BINARY_DIR}/Debug")
	endif()
else ()
	FILE(COPY Resources/splashscreen.png DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY Resources/openIcon.png DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY Resources/screenshotIcon.png DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY Resources/Group32ModelViewerLogo.ico DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY Resources/logo.stl DESTINATION "${CMAKE_BINARY_DIR}")
endif ()
