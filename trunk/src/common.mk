GAME_DATA_PATH="\"./\""

CC = g++

CFLAGS = -Wall -Werror -g3 -DGAME_DATA_PATH=$(GAME_DATA_PATH) -I../vendor/assimp/include

COMPILE_COMMAND = $(CC) $(CFLAGS)

LINK_COMMAND = g++ -Wall -Werror -g3 $(LINKER_LIBRARIES) -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lGLEW


