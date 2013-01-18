include ../common.mk

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

