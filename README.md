# Group32's 'TeapotViewer'  [![Build Status](https://travis-ci.com/Edreva/TeapotViewer.svg?token=KfjpPmpzoQSKxZtRyAzE&branch=master)](https://travis-ci.com/Edreva/TeapotViewer)  
TeapotViewer is 3D Model Viewer for Windows that can load .STL and .mod files. Written in C++, its GUI uses the Qt widget toolkit and employs VTK for the model visualisation. Requires Visual C++ to run. Produced by [Group 32](TEAM.md).
## [Documentation](https://edreva.github.io/TeapotViewer/ "Library Documentation")  
## [Installers](https://github.com/edreva/TeapotViewer/tree/gh-resources/Installers "Installers")
## Demo  
![](https://github.com/edreva/2019_GROUP_32/blob/gh-resources/ModelViewerDemo.gif) 
## Building Source Code  
**1. Create build directory in root directory**
```cmd
mkdir build
```
**2. Generate cmake files in build directory - Must ensure that CMake is passed the location of Qt and VTK binaries and they are in PATH**  
*Example using MSVC*
```cmd
cd build
cmake -DCMAKE_PREFIX_PATH=C:\path\to\Qt\bin;C:\path\to\VTK\bin" -G "Visual Studio 16 2019" ..
```
*Example using MinGW*
```cmd
cd build
cmake -DCMAKE_PREFIX_PATH=C:\path\to\Qt\bin;C:\path\to\VTK\bin" -G "MinGW Makefiles" ..
```
**3. Generate binaries using chosen compiler**  
*Example using MSVC  -- Cannot yet be done in release, see Known Bugs and Issues*
```cmd
msbuild Group32ModelViewer.sln
```
*Example using MinGW*
```cmd
mingw32-make
```
## Install (Source) 
*Example using MSVC -- Cannot yet be done in release, see Known Bugs and Issues*
```cmd
msbuild INSTALL.vcxproj
```
*Example using MinGW*
```cmd
mingw32-make install
```
## Install (NSIS)
### Build Installer
*Example using MSVC -- Cannot yet be done in release, see Known Bugs and Issues*
```cmd
msbuild PACKAGE.vcxproj
```
*Example using MinGW*
```cmd
mingw32-make package
```
### Running Installer
![](https://github.com/edreva/2019_GROUP_32/blob/gh-resources/ModelViewerNSISDemo.gif) 
## Known Bugs and Issues  
- When building from source using MinGW, a quoting issue is encountered when trying to load the .rc file to give the executable an icon. By default, this is avoided by not adding the resource.rc file as a source if a GNU compiler is detected. However, there is an odd fix. By running mingw32-make once then, once this fails, change the root CMakeLists.txt (in any way so just add whitespace and save) then running mingw32-make again will run without error.
- Currently there are issues when trying to use MSVC to build release versions of the project. The executable seems to be linked to Qts debug dlls even though it was compiled in Release. Suspected linker issue.
