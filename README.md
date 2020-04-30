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
