#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

using namespace sf;

class GameState;									// Forward declaration

enum class StateType {
	MENU,
	GROWING,
	SHRINKING,
	ALTERNATING,
	RANDOM,
	GAME_OVER
};

class Engine {
private:
	RenderWindow m_Window;							// Window made when Engine object created
	Font m_Font;									// Font object loaded in engine
	std::unique_ptr<GameState> m_CurrentState;		// Connector variable for game states - COMPOSITION

public:
	Engine();										// Constructor
	void setFont(GameState& state);					// COME BACK LATER HERE
	Font& getFont() { return m_Font; }				// COME BACK LATER HERE
	GameState& changeState(StateType type);			// Returns a reference for which game state is in play
	void run();										// 

};
#endif