#ifndef	__MENU_H__
#define __MENU_H__

class Menu {
 private:
	bool displaymenu;
	bool start;
	int story;
 public:
	 Menu();
	~Menu();
	void Initialize(void);
	void Quit(void);
	void Render(void);
	void MenuProcessEvents(void);
	void Story(void);
};

#endif				/* __MENU_H__ */
