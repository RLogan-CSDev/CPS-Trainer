#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class UIManager {
private:
	Font m_Font;
	View m_HUD;
	vector<Text> m_MenuElements;
	vector<Text> m_GameplayElements;

public:
	UIManager(Font& font, RenderWindow& window);
	void drawUI(RenderWindow& window, bool isMenu);
	void createMainMenu(RenderWindow& window);
};
#endif