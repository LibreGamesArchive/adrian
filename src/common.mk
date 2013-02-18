GAME_DATA_PATH="\"./\""

CC = g++

CFLAGS = -DDEBUG -Wall -Werror -g3 -DLINUX -DGAME_DATA_PATH=$(GAME_DATA_PATH) -I../vendor/assimp/include `xml2-config --cflags`

COMPILE_COMMAND = $(CC) $(CFLAGS)

LINK_COMMAND = g++ -Wall -Werror -g3 $(LINKER_LIBRARIES) -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lGLEW `xml2-config --libs`


