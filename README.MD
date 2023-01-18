# Simple _hopefully PBR_ openGL renderer

![Screenshot of program](Window.png)

This is following the guidance of [learnopengl](https://learnopengl.com/).

## Tech:
- [GLFW](https://www.glfw.org/documentation.html) as window creation and openGL interface
- [GLAD](https://glad.dav1d.de/) as a managment system, simplifying openGL featuresets
- [stb](https://github.com/nothings/stb/blob/master/stb_image.h) image format library
- [glm](http://glm.g-truc.net/0.9.8/index.html) openGL focused mathematics library
- [ASSIMP](https://github.com/assimp/assimp) library to handle various 3d model formats
- [VS Code](https://code.visualstudio.com/) is being used as the IDE
- [mingw32](https://www.msys2.org/) g++ and gdb provided via mingw from msys
- [git](https://git-scm.com/) VCS using git within VS Code

## Goals:
- [x] Load openGL
- [x] Load models
- [ ] Lighting
- [ ] Shadows
- [ ] PBR materials

## Setup:
### VS Code
Currently, I am compiling this code in the VS Code IDE, with mingw supplying g++. Below is the arguments in tasks.json:
```JSON
"args": [
    "-fdiagnostics-color=always",
    "-g",
    "${file}",
    "..\\include\\glad.c",
    "-lglfw3","-lassimp","-lz","-lgdi32","-lopengl32","-luser32","-lkernel32",
    "-IS:\\Mutual\\Code\\C++\\Libraries\\glfw-3.3.8\\include",
    "-LS:\\Mutual\\Code\\C++\\Libraries\\glfw-3.3.8\\build\\src",
    "-IS:\\Mutual\\Code\\C++\\Libraries\\glad\\include",
    "-IS:\\Mutual\\Code\\C++\\Libraries\\glm",
    "-IS:\\Mutual\\Code\\C++\\oGL\\include",
    "-IS:\\Mutual\\Code\\C++\\Libraries\\assimp\\include",
    "-IS:\\Mutual\\Code\\C++\\Libraries\\assimp\\build\\include",
    "-LS:\\Mutual\\Code\\C++\\Libraries\\assimp\\build\\lib",                
    "-o",
    "${fileDirname}\\${fileBasenameNoExtension}.exe"
],
```
For intellisense, my c_cpp_properties.json has:
```JSON
"includePath":[
    "${workspaceFolder}/**",
    "S:\\Mutual\\Code\\C++\\Libraries\\glfw-3.3.8\\include",
    "S:\\Mutual\\Code\\C++\\Libraries\\glad\\include",
    "S:\\Mutual\\Code\\C++\\Libraries\\glm\\**",
    "S:\\Mutual\\Code\\C++\\Libraries\\assimp\\include",
    "S:\\Mutual\\Code\\C++\\Libraries\\assimp\\build\\include"
],
```
### GLFW
GLFW was built using its supplied CMAKE config. The library is then statically compiled into this project. If changing to dynamic linking, or chaging the includes, be aware that GLFW requires specific ```#define``` statements, and ordering of ```#include``` lines.
### GLAD
GLAD is customised via their website. A ```glad.c``` file is then to be built into the project.
### STB
This header library is downloaded, then included into ```model.h```. This library demands the definition ```#define STB_IMAGE_IMPLEMENTATION``` placed before the ```#include``` line. This library must also only be included once within the project (the compiler complains of redefinition if the ```#include``` is in multiple files). The file had its include moved from ```main.cpp``` to ```model.h``` as that is where the texture handling code now resides.
### GLM
Another larger library. Downloaded, and then linked to for g++ to find at compile time.
### ASSIMP
A library to handle the vast world of 3d file formats. Two compiler terms were inserted into the config file to stop some warnings from being treated as errors and halting compilation. In ```CMakeLists.txt```, within the grouped compiler settings; ``` -Wno-error=array-bounds -Wno-error=maybe-uninitialized``` was placed int the ```MINGW``` section of setting the ```CMAKE_CXX_FLAGS```. This is lines 306 and 308 at time of writing.

The config file was also changed to turn off ```BUILD_SHARED_LIBS``` & ```ASSIMP_INSTALL```, and enabling ```ASSIMP_BUILD_ZLIB```.

A build folder was used for the library. CMAKE was run with: ```cmake .\CMakeLists.txt -B build -G "MinGW Makefiles"```