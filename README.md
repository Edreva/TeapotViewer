# 3D Model Viewer Project  [![Build Status](https://travis-ci.com/EDrever-Smith/2019_GROUP_32.svg?token=KfjpPmpzoQSKxZtRyAzE&branch=master)](https://travis-ci.com/EDrever-Smith/2019_GROUP_32)  
**Documentation available at: https://edrever-smith.github.io/2019_GROUP_32/**  
## Demo  
![](ModelViewerDemo.gif) 
## Building Source Code  
**1. Create build directory in root directory**
```cmd
mkdir build
```
**2. Generate cmake files in build directory - Must ensure that CMake is passed the location of Qt and VTK binaries**  
*Example using MSVC*
```cmd
cd build
cmake -DCMAKE_PREFIX_PATH=C:\path\to\Qt\bin;C:\path\to\VTK\bin" -G "Visual Studio 16 2019" ..
```
**3. Generate binaries using chosen compiler**  
*Example using MSVC*
```cmd
msbuild Group32ModelViewer.sln
```
## Install (Source)  
## Install (NSIS)
--insert gif walkthrough here
## Known Bugs and Issues  
-When building from source using MinGW, a quoting issue is encountered when trying to load the .rc file to give the executable an icon. By default, this is avoided by not adding the resource.rc file as a source if a GNU compiler is detected. However, there is an odd fix. By running mingw32-make once then, once this fails, change the root CMakeLists.txt (in any way so just add whitespace and save) then running mingw32-make again will run without error.
