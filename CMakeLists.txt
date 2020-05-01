#This CMakeLists.txt generates the main application's executable as well as calls the other cmakelists to run
cmake_minimum_required(VERSION 3.12.0)

foreach(p
    CMP0071 # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

#BundleUtilities cannot be included at configure time.
cmake_policy(SET CMP0080 NEW)

project(Group32ModelViewer)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

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


#CMake trys to be clever with .rc file but ends up screwing up the working directory, also kinda a mingw bug. The commented code belowe was my attempt to solve this issue but to no avail.
#if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  #set(CMAKE_RC_COMPILER windres.exe)
#endif()

#Because of issue with .rc files and mingw bug, if GNU compiler used then no icons unfortunately
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
else()
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
      Model_Viewer_Application/resources/resource.rc
  		${UI_Srcs}
  		${QRC_Srcs}
  		)
endif()
# Tell CMake that the executable depends on our class library
target_link_libraries( ModelViewerApp Model_Loading_Library )

# Tell CMake that the executable depends on the Qt::Widget libraries.
target_link_libraries( ModelViewerApp Qt5::Widgets )

# Tell CMake that the executable depends on the VTK libraries
target_link_libraries( ModelViewerApp ${VTK_LIBRARIES} )

#find all the applications runtime dependencies and add them to a list that can be used by fixup bundle later
get_target_property(QT_CORE_LIB Qt5::Core LOCATION)
get_filename_component(QT_RUNTIME_DIR "${QT_CORE_LIB}" DIRECTORY)
list(APPEND LIBS_PATH "${QT_RUNTIME_DIR}")
list(APPEND LIBS_PATH "${VTK_RUNTIME_LIBRARY_DIRS}")

#install the main application
install(TARGETS ModelViewerApp
  RUNTIME DESTINATION bin
  COMPONENT Application)
#install the necessary resources (icons)
install(DIRECTORY Model_Viewer_Application/resources/
  DESTINATION bin
  COMPONENT Application)
#install models as a different component - optional if user wants the model files
install(DIRECTORY Model_Viewer_Application/models/
  DESTINATION models
  COMPONENT Models)
install(FILES README.md
  DESTINATION .
COMPONENT Application)

#set the keword APP to be the directory to the installed executable to be fixed up.
set(APP \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/ModelViewerApp${CMAKE_EXECUTABLE_SUFFIX})

#Ensure that all dependencies are installed
install(CODE "
  include(InstallRequiredSystemLibraries)
  include(BundleUtilities)
  fixup_bundle(\"${APP}\" \"\" \"${LIBS_PATH}\")
  execute_process(COMMAND windeployqt.exe --release \"${APP}\")"
  COMPONENT Application)

#Choose NSIS if on windows or zip otherwise
if(WIN32)
set(CPACK_GENERATOR NSIS)
else()
set(CPACK_GENERATOR ZIP)
endif()

#Set NSIS installer details
set(CPACK_PACKAGE_NAME "Group32ModelViewer")
set(CPACK_PACKAGE_VENDOR "CMake.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Group32 Model Viewer - Open and Analyse .STL and .MOD files")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Group32ModelViewer")
#Location of icon is different depending on compiler so ensure we're looking in the right place
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CPACK_NSIS_MUI_ICON "../../../Release/Group32ModelViewerLogo.ico")
  else()
      set(CPACK_NSIS_MUI_ICON "../../../Debug/Group32ModelViewerLogo.ico")
  endif()
else ()
  set(CPACK_NSIS_MUI_ICON "../../../Group32ModelViewerLogo.ico")
endif()

INCLUDE(CPack)
