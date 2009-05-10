COMPILE_COMMAND = g++ -Wall -Werror -g3 -I../../vendor/assimp/include
#COMPILE_COMMAND = g++ -Wall -Werror -O3

LINK_COMMAND = g++ -Wall -Werror -g3 -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer
#LINK_COMMAND = g++ -Wall -Werror -O3 -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer

OBJECT_FILES	=	menuitem.o			\
					menupage.o			\
					menu.o				\

TGT	=	menu.a
	
$(TGT): $(OBJECT_FILES)
	ar rcs menu.a menu.o menuitem.o menupage.o
	
menu.o: menu.cpp 
	$(COMPILE_COMMAND) -c menu.cpp -o menu.o

menuitem.o:	menuitem.cpp menuitem.h
	$(COMPILE_COMMAND) -c menuitem.cpp -o menuitem.o

menupage.o:	menupage.cpp menupage.h
	$(COMPILE_COMMAND) -c menupage.cpp -o menupage.o

clean:
	-rm -f $(OBJECT_FILES) $(TGT)

