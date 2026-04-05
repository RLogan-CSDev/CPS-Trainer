#ifndef GROWINGSTATE_H
#define GROWINGSTATE_H

#include <GameState.h>

using namespace sf;

class GrowingState : public GameState {
	/*
	* CHILD CLASS - GrowingState overrides the behavior of the GameState functions
	* 
	* Purpose: Allows the pure virtual functions to have generate behavior
	* for the necessary game mode selected by the player. 
	*/
private:
	CircleShape m_Circle;				// Game object
	float m_TimerValue = 15.0f;			// Game timer
	int m_ScoreValue = 0;				// Score
	Vector2f m_Scale;					// Scale, used in multiple functions

public:
	~GrowingState() override;						// Logs change in game state
	void init(RenderWindow& window) override;		// Initializes the circle for the game
	void update(float elapsedTime) override;
	void draw() override;
};
#endif