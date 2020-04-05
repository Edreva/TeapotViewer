cmake_minimum_required(VERSION 3.12.0)

foreach(p
    CMP0071 # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

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

#Add subdirectories containing cmake lists
add_subdirectory("Model_Loading_Library")
add_subdirectory("Model_Viewer_Application")

#Create the executable using application source files
add_executable( ModelViewerApp MACOSX_BUNDLE
		Model_Viewer_Application/main.cpp 
		Model_Viewer_Application/mainwindow.cpp
		Model_Viewer_Application/mainwindow.h
		Model_Viewer_Application/mainwindow.ui	
    		Model_Viewer_Application/dialogeditshrinkfilter.h 									Model_Viewer_Application/dialogeditshrinkfilter.cpp
		Model_Viewer_Application/dialogeditshrinkfilter.ui
		${UI_Srcs}
		${QRC_Srcs}
		)

target_link_libraries( ModelViewerApp Model_Loading_Library )

# Tell CMake that the executable depends on the Qt::Widget libraries.
target_link_libraries( ModelViewerApp Qt5::Widgets )

# Tell CMake that the executable depends on the VTK libraries
target_link_libraries( ModelViewerApp ${VTK_LIBRARIES} )