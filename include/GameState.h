#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class GameState {
	/*
	* PARENT CLASS - GameState utilizes inheritance and polymorphic behavior to control game states
	* 
	* Purpose: Maintain modular code that doesn't repeat itself several times over, to create a space
	* where changes can be made in one place instead of having to look in several locations, decreasing
	* the time spent debugging. 
	*/
private:
	// No private variables or functions for this base class

public:
	virtual ~GameState() {}								// Virtual destructor allows for compiler to backtrack all game modes to clean up memory
	virtual void handleInput(RenderWindow& window);		// NOT pure virtual as some modes do not need to be overridden for input
	virtual void update(float elapsedTime) = 0;			// Updates game logic
	virtual void draw() = 0;							// Draws/renders objects to screen
};
#endif