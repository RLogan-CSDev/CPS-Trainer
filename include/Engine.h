#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <GameState.h>

using namespace sf;

class Engine {
private:
	RenderWindow m_Window;
	Font m_Font;
	std::unique_ptr<GameState> m_CurrentState;

public:
	Engine();
	Font& getFont() { return m_Font; }
	void run();
};


#endif