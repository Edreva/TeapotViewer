# 3D Model Viewer Project  [![Build Status](https://travis-ci.com/EDrever-Smith/2019_GROUP_32.svg?token=KfjpPmpzoQSKxZtRyAzE&branch=master)](https://travis-ci.com/EDrever-Smith/2019_GROUP_32)  
**Documentation available at: https://edrever-smith.github.io/2019_GROUP_32/**  
## Demo  
![](ModelViewerDemo.gif) 
## Setup (from Clone)
**1. Create build directory in root directory**
```cmd
mkdir build
```
**2. Generate cmake files in build directory**  
*Example using MSVC*
```cmd
cd build
cmake -G "Visual Studio 16 2019" ..
```
**3. Generate binaries using chosen compiler**  
*Example using MSVC*
```cmd
msbuild Group32ModelViewer.sln
```
