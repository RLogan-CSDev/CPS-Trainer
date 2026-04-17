#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace sf;

/*
* THIS IS THE VERSION 2 OF THE CPS TRAINER
* 
* The purpose of this version is to better organize the working state of
* the game into...
* ... First, a long main() function with all game logic nested within.
* ... Second, delegating specific sets of logic to functions.
*/

int main() {

	// 1. Create a window for the game.
	VideoMode pixelGraph(Vector2u(1280, 720));
	RenderWindow gameWindow(pixelGraph, "CPS Trainer",
		Style::Close | Style::Resize | Style::Titlebar);

	// 2. Create the core game object, a circle!
	CircleShape gameCircle;
	gameCircle.setFillColor(Color::White);					// White = default
	float radius = gameCircle.getRadius();					// Obtain radius from object made
	gameCircle.setOrigin({ radius, radius });				// Setting origin helps set position
	gameCircle.setPosition({ gameWindow.getSize().x / 2.f,	
		gameWindow.getSize().y / 2.f });					// Sets circle position in center of screen

	// 3. Load the font.
	Font mainFont;
	if (!mainFont.openFromFile("assets/fonts/Cavalier.ttf")) {
		// If the font does not load from the files, an error message prints to console.
		cout << "[DEBUG] - Font failed to load.\n";
	}

	// 4. Create Text objects.
	// 1 - Title Text
	Text titleText(mainFont, "CPS Trainer", 55);			// Title of game for the menu
	FloatRect titleBounds = titleText.getLocalBounds();		// Pulls the size of the title text object
	titleText.setOrigin({
		titleBounds.position.x + titleBounds.size.x / 2.f,	// Top left + top right distance / 2 = half
		titleBounds.position.y + titleBounds.size.y / 2.f	// Top left + bottom left distance / 2 = half
		});													// Origin is set to the center
	titleText.setPosition({ gameWindow.getSize().x / 2.f,
		gameWindow.getSize().y / 3.f });					// Places title in horizontal middle, 1/3rd from top
	// 2 - Options Text
	Text optionsText(mainFont, "Press '1' for Growing Mode\n\n"
		"Press '2' for Shrinking Mode\n\n"
		"Press '3' for Alternating Mode\n\n"
		"Press '4' for Random Mode", 40);
	FloatRect optionBounds = optionsText.getLocalBounds();
	optionsText.setOrigin({
		optionBounds.position.x + optionBounds.size.x / 2.f,
		optionBounds.position.y + optionBounds.size.y / 2.f
		});
	optionsText.setPosition({ titleText.getPosition().x,
		titleText.getPosition().y * 2.f });					// Places options in horizontal middle, 2/3rds from top
	// 3 - Timer Text
	Text timerText(mainFont, "Timer: ", 25);
	FloatRect timerBounds = timerText.getLocalBounds();
	timerText.setOrigin({
		timerBounds.position.x + timerBounds.size.x / 2.f,
		timerBounds.position.y + timerBounds.size.y / 2.f
		});
	timerText.setPosition({ gameWindow.getSize().x / 3.f,
		gameWindow.getSize().y / 10.f });					// Places timer 1/3rd horizontal distance from left, 1/10th from top
	// 4 - CPS Text
	Text cpsText(mainFont, "CPS: ", 25);
	FloatRect cpsBounds = cpsText.getLocalBounds();
	cpsText.setOrigin({
		cpsBounds.position.x + cpsBounds.size.x / 2.f,
		cpsBounds.position.y + cpsBounds.size.y / 2.f
		});
	cpsText.setPosition({ gameWindow.getSize().x / 3.f * 2.f,
		gameWindow.getSize().y / 10.f });					// Places CPS 2/3rds horizontally distance from left, 1/10th from top
	// 5 - Average Calculation Text
	Text calcCPSText(mainFont, "Average: ", 35);
	FloatRect calcCPSBounds = calcCPSText.getLocalBounds();
	calcCPSText.setOrigin({
		calcCPSBounds.position.x + calcCPSBounds.size.x / 2.f,
		calcCPSBounds.position.y + calcCPSBounds.size.y / 2.f
		});
	calcCPSText.setPosition({ gameWindow.getSize().x / 2.f,
		gameWindow.getSize().y / 2.f });					// Places average calculation in exact center of screen
	calcCPSText.setFillColor(Color::Black);					// Sets color of this text to black
	calcCPSText.setOutlineColor(Color::White);				// Sets outline color
	calcCPSText.setOutlineThickness(5.f);					// Sets thickness of outline to 5 pixels thick

	// 5. Create State Machine
	enum class State {
		MENU,
		GROWING,
		SHRINKING,
		ALTERNATING,
		RANDOM,
		GAME_OVER
	};
	State gameState = State::MENU;							// Game will start in MENU state

	// 6. Other variables needed
	int cpsValue = 0;
	bool waitingForLeftClick = true;						// Needed for ALTERNATING state
	srand((int)time(0));									// Needed for RANDOM state
	Clock gameClock;
	Time dt = gameClock.restart();
	float deltaTime = dt.asSeconds();

	// 7. Game Loop
	while (gameWindow.isOpen()) {
		/*
		* INPUT - Capture keyboard/mouse/window interactions
		*/
		optional event = gameWindow.pollEvent();
		while (event) {
			if (event->is<Event::Closed>() ||
				(event->is<Event::KeyPressed>() &&
					event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)) {
				gameWindow.close();
			}
			// Separate if structures since game state changes based on input vs. window interaction
			if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num1) {
				gameState = State::GROWING;
			}
			else if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num2) {
				gameState = State::SHRINKING;
			}
			else if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num3) {
				gameState = State::ALTERNATING;
			}
			else if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num4) {
				gameState = State::RANDOM;
			}
		}


		/*
		* UPDATE - Game Logic
		*/
		switch (gameState) {
		case State::MENU:
			break;
		case State::GROWING:
			gameCircle.setRadius(45.f);
			gameCircle.setOrigin({ radius, radius });
			if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
				if (mouseEvent->button == Mouse::Button::Left) {
					// Detects left click
					Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
					// Allows for the mouse movements to be detected
					if (gameCircle.getGlobalBounds().contains(worldPos)) {
						// Only when the player clicks on the circle
						gameCircle.setFillColor(Color::Blue);

					}
				}
			}
		}


		/*
		* RENDER - Clears screen and draws objects
		*/


	}

	return 0;
}