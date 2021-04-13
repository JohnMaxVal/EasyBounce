CC = gcc
GDB_FLAG = -g

SDL_INCLUDE_PATH = -IC:/mingw_dev_lib/include/SDL2
SDL_LIBS_PATH = -LC:/mingw_dev_lib/lib

# SDL options
COMPILER_FLAGS = -Wall -std=c99
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

SRC_PATH = ./src/*.c

DEBUG_PATH = ./Debug
RELEASE_PATH = ./Release

SDL_DLL_PATH = C:/mingw_dev_lib/bin/SDL2.dll

build:
	$(CC) \
	$(COMPILER_FLAGS) \
	$(SRC_PATH) \
	$(SDL_INCLUDE_PATH) $(SDL_LIBS_PATH) $(LINKER_FLAGS) \
	-o $(RELEASE_PATH)/game

run:
	./game

clear:
	rm game