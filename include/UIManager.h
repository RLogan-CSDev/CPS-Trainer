#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>

using namespace sf;

class UIManager {
private:
	Font m_Font;
	View m_HUD;

public:
	UIManager(Font& font, RenderWindow& window);
	void drawUI();
};
#endif