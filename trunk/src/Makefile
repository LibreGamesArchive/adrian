COMPILE_COMMAND = g++ -Wall -Werror -g3

LINK_COMMAND = g++ -Wall -Werror -g3 -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer

OBJECT_FILES	=	Map.o			\
					MiniMap.o		\
					Camera.o		\
					Building.o		\
					Hero.o		\
					Guard.o		\
					main.o			\
					Md2.o			\
					Tga.o			\
					Font.o			\
					Globals.o		\
					Menu.o			\
					Cursor.o		\
					Panel.o			\
					SoundSystem.o	\

	
adrian: $(OBJECT_FILES) Menu/menu.a
	$(LINK_COMMAND) $(OBJECT_FILES) Menu/menu.a -o adrian
	
Menu/menu.a:
	make -C Menu

Menu.o: Menu.cpp Menu.h
	$(COMPILE_COMMAND) -c Menu.cpp -o Menu.o

Panel.o: Panel.cpp Panel.h
	$(COMPILE_COMMAND) -c Panel.cpp -o Panel.o

SoundSystem.o: SoundSystem.cpp SoundSystem.h
	$(COMPILE_COMMAND) -c SoundSystem.cpp -o SoundSystem.o

Font.o: Font.cpp 
	$(COMPILE_COMMAND) -c Font.cpp -o Font.o

Md2.o: Md2.cpp 
	$(COMPILE_COMMAND) -c Md2.cpp -o Md2.o

main.o:	main.cpp main.h
	$(COMPILE_COMMAND) -c main.cpp -o main.o

Map.o:	Map.cpp Map.h
	$(COMPILE_COMMAND) -c Map.cpp -o Map.o

MiniMap.o:	MiniMap.cpp MiniMap.h
	$(COMPILE_COMMAND) -c MiniMap.cpp -o MiniMap.o

Camera.o:	Camera.cpp Camera.h
	$(COMPILE_COMMAND) -c Camera.cpp -o Camera.o

Building.o:	Building.cpp Building.h
	$(COMPILE_COMMAND) -c Building.cpp -o Building.o

Hero.o:	Hero.cpp Hero.h Md2.h Md2.o
	$(COMPILE_COMMAND) -c Hero.cpp -o Hero.o

Guard.o:	Guard.cpp Guard.h Md2.h Md2.o
	$(COMPILE_COMMAND) -c Guard.cpp -o Guard.o

Globals.o:	globals.cpp globals.h
	$(COMPILE_COMMAND) -c globals.cpp -o Globals.o
	
Cursor.o:	cursor.c cursor.h
	$(COMPILE_COMMAND) -c cursor.c -o Cursor.o

Tga.o:	tga.c tga.h
	$(COMPILE_COMMAND) -c tga.c -o Tga.o
	
clean:
	make -C Menu clean
	-rm -f $(OBJECT_FILES) adrian

c:
	-rm -f $(OBJECT_FILES) adrian
