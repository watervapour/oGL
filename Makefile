CC = g++.exe

FILES = .\src\main.cpp .\include\glad.c

LIB_BASE_PATH = S:\Mutual\Code\C++\libraries
INCLUDE_PATHS = -I.\include \
				-I$(LIB_BASE_PATH)\glfw-3.3.8-win\include \
				-I$(LIB_BASE_PATH)\glad\include \
				-I$(LIB_BASE_PATH)\glm \
				-I$(LIB_BASE_PATH)\assimp\include\ \
				-I$(LIB_BASE_PATH)\assimp\build\include 
LINK_PATHS =	-L$(LIB_BASE_PATH)\glfw-3.3.8-win\build\src \
				-L$(LIB_BASE_PATH)\assimp\build\lib \

COMPILER_FLAGS = -Wall -D_PLAT_WIN
LINKER_FLAGS = -lglfw3 -lassimp -lz -lgdi32 -lopengl32 -luser32 -lkernel32
DEBUG_FLAGS = -g

APP_NAME = ./oGL.exe
APP_DEBUG_NAME = ./oGL-debug.exe

all: $(FILES)
	$(CC) $(FILES) $(INCLUDE_PATHS) $(LINK_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(APP_NAME)

debug: $(FILES)
	$(CC) $(FILES) $(INCLUDE_PATHS) $(LINK_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(DEBUG_FLAGS) -o $(APP_DEBUG_NAME)
