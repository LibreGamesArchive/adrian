include common.mk

LINKER_LIBRARIES = ../vendor/assimp/bin/gcc/libassimp.so

OBJECT_FILES	=	Map.o			\
					MiniMap.o		\
					Camera.o		\
					Building.o		\
					Hero.o		\
					Guard.o		\
					main.o			\
					Game.o			\
					Md2.o			\
					TTFFont.o		\
					TextObject.o		\
					texture.o		\
					Sprite.o		\
					Font.o			\
					Globals.o		\
					Cursor.o		\
					Panel.o			\
					SoundSystem.o	\
					platform.o		\
					staticloader.o	\
					RenderPass.o	\
					ShaderProgram.o	\
					PPShaderProgram.o	\
					SMShaderProgram.o	\

adrian: $(OBJECT_FILES) Menu/menu.a
	$(LINK_COMMAND) $(OBJECT_FILES) Menu/menu.a -o adrian
	
Menu/menu.a: Menu/*.cpp Menu/*.h
	make -C Menu

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

Game.o:	Game.cpp Game.h
	$(COMPILE_COMMAND) -c Game.cpp -o Game.o

Sprite.o:	Sprite.cpp Sprite.h
	$(COMPILE_COMMAND) -c Sprite.cpp -o Sprite.o

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
	
TextObject.o:	TextObject.cpp TextObject.h
	$(COMPILE_COMMAND) -c TextObject.cpp -o TextObject.o

TTFFont.o:	TTFFont.cpp TTFFont.h
	$(COMPILE_COMMAND) -c TTFFont.cpp -o TTFFont.o

Cursor.o:	cursor.cpp cursor.h
	$(COMPILE_COMMAND) -c cursor.cpp -o Cursor.o

texture.o:	texture.cpp texture.h
	$(COMPILE_COMMAND) -c texture.cpp -o texture.o
	
staticloader.o:	staticloader.cpp staticloader.h
	$(COMPILE_COMMAND) -c staticloader.cpp -o staticloader.o
	
platform.o:	platform.cpp platform.h
	$(COMPILE_COMMAND) -c platform.cpp -o platform.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	make -C Menu clean
	-rm -f $(OBJECT_FILES) adrian

c:
	-rm -f $(OBJECT_FILES) adrian
