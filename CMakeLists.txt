#This CMakeLists.txt generates the main application's executable as well as calls the other cmakelists to run
cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
#set(CMAKE_BUILD_TYPE Release)
foreach(p
    CMP0071 # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

cmake_policy(SET CMP0080 NEW)

project(Group32ModelViewer)

# The CMake build process might generate some new files in the current
# directory. This makes sure they can be found.
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# This allows CMake to run one of Qt's build tools called moc
# if it is needed. moc.exe can be found in Qt's bin directory.
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

# Find the Qt widgets package. This locates the relevant include and
# lib directories, and the necessary static libraries for linking.
find_package( Qt5Widgets )

set( UIS mainwindow.ui )
qt5_wrap_ui( UI_Srcs ${UIS} )

# Also link to VTK
find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

#Add subdirectories containing cmake lists and runs their contents
add_subdirectory("Model_Loading_Library")
add_subdirectory("Model_Viewer_Application")

#Create the executable using application source files
add_executable( ModelViewerApp MACOSX_BUNDLE
		Model_Viewer_Application/main.cpp
		Model_Viewer_Application/mainwindow.cpp
		Model_Viewer_Application/mainwindow.h
		Model_Viewer_Application/mainwindow.ui
    Model_Viewer_Application/dialogeditshrinkfilter.h
		Model_Viewer_Application/dialogeditshrinkfilter.cpp
		Model_Viewer_Application/dialogeditshrinkfilter.ui
    Model_Viewer_Application/dialogeditcurvaturefilter.h
		Model_Viewer_Application/dialogeditcurvaturefilter.cpp
		Model_Viewer_Application/dialogeditcurvaturefilter.ui
		${UI_Srcs}
		${QRC_Srcs}
		)
# Tell CMake that the executable depends on our class library
target_link_libraries( ModelViewerApp Model_Loading_Library )

# Tell CMake that the executable depends on the Qt::Widget libraries.
target_link_libraries( ModelViewerApp Qt5::Widgets )

# Tell CMake that the executable depends on the VTK libraries
target_link_libraries( ModelViewerApp ${VTK_LIBRARIES} )

get_target_property(QT_CORE_LIB Qt5::Core LOCATION)

get_filename_component(QT_RUNTIME_DIR "${QT_CORE_LIB}" DIRECTORY)

list(APPEND LIBS_PATH "${QT_RUNTIME_DIR}")

list(APPEND LIBS_PATH "${VTK_RUNTIME_LIBRARY_DIRS}")

install(TARGETS ModelViewerApp
RUNTIME DESTINATION bin
#CONFIGURATIONS Release
COMPONENT Runtime)

include (InstallRequiredSystemLibraries)

install(DIRECTORY Model_Viewer_Application/resources/
DESTINATION bin
COMPONENT Runtime)

install(DIRECTORY Model_Viewer_Application/models/
  DESTINATION models
  COMPONENT Models)

set(APPS \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/ModelViewerApp${CMAKE_EXECUTABLE_SUFFIX})
install(CODE "
  include(BundleUtilities)
  fixup_bundle(\"${APPS}\" \"\" \"${LIBS_PATH}\")"
  COMPONENT Runtime)

if(WIN32)
set(CPACK_GENERATOR NSIS)
else()
set(CPACK_GENERATOR ZIP)
endif()
INCLUDE(CPack)
