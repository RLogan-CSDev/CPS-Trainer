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
	/*
	CircleShape m_Circle;				// Game object
	float m_TimerValue = 15.0f;			// Game timer
	int m_ScoreValue = 0;				// Score
	Vector2f m_Scale;					// Scale, used in multiple functions
	const float m_Radius = 50.f;		// Constant for radius
	const float m_GrowthRate = 1.05f;	// Constant for rate of growth
	*/

public:
	GrowingState();
	GrowingState(RenderWindow& window);				// Constructor that sets up the circle
	~GrowingState() override;						// Logs change in game state
	void update(float elapsedTime) override;		// Contains all logic for play
	void draw() override;							// 
};
#endif