#This CMakeLists.txt creates a library containing all the classes our application relies on, as well as generates our test executables.
cmake_minimum_required(VERSION 3.12.0)

project(Group32ModelViewer)

#Makes sure compiler knows to use c++11 - Will throw warning on some compilers but was required for compiler that Travis uses
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Allows unit tests to be created
include (CTest)

#adds all the classes to a library
add_library(Model_Loading_Library
 src/cell.cpp include/cell.h
 src/material.cpp include/material.h
 src/matrix.cpp include/matrix.h
 src/vector.cpp include/vector.h
 src/model.cpp include/model.h
 src/hexahedron.cpp include/hexahedron.h
 src/pyramid.cpp include/pyramid.h
 src/tetrahedron.cpp include/tetrahedron.h)

#Notifies CMake that anything using Model_Loading_Library is dependent on the include directory
target_include_directories(Model_Loading_Library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

#identify mains of executables
add_executable(modelTest test/modelTest.cpp)
add_executable(matrixTest test/matrixTest.cpp)
add_executable(vectorTest test/vectorTest.cpp)

#links required libraries
target_link_libraries(modelTest  Model_Loading_Library)
target_link_libraries(matrixTest Model_Loading_Library)
target_link_libraries(vectorTest Model_Loading_Library)

#Marks the executables as unit tests
add_test(vectorTest vectorTest)
add_test(matrixTest matrixTest)
add_test(modelTest modelTest)

#Copies the testfiles into the binary directory.
#MSVC puts binaries inside Debug/Release subdirectory whilst other known compilers do not. The following conditional checks the compiler being used and ensures that the necessary resources are copied to the correct location.
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	FILE(COPY test/ExampleModel1.mod DESTINATION "${CMAKE_BINARY_DIR}/Debug")
	FILE(COPY test/ExampleModel2.mod DESTINATION "${CMAKE_BINARY_DIR}/Debug")
	FILE(COPY test/ExampleModel3.mod DESTINATION "${CMAKE_BINARY_DIR}/Debug")
	FILE(COPY test/GoldenFile.mod DESTINATION "${CMAKE_BINARY_DIR}/Debug")
else ()
	FILE(COPY test/ExampleModel1.mod DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY test/ExampleModel2.mod DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY test/ExampleModel3.mod DESTINATION "${CMAKE_BINARY_DIR}")
	FILE(COPY test/GoldenFile.mod DESTINATION "${CMAKE_BINARY_DIR}")
endif ()
