#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

/*
* THIS IS THE VERSION 3 (Phase 2) OF THE CPS TRAINER
*
* The purpose of this version is to better organize the working state of
* the game into...
* ... First, a long main() function with all game logic nested within.
* ... Second, delegating specific sets of logic to functions.
*/

// Function prototypes
void initializeTextObjects(vector<Text>& textObj, float winX, float winY);
void growingCircleMode(RenderWindow& win, CircleShape& circle, Text& text, Vector2f& scale, Vector2f& pos, float radius, int& cps);
void shrinkingCircleMode(RenderWindow& win, CircleShape& circle, Text& text, Vector2f& scale, Vector2f& pos, float radius, int& cps);
void alternatingCircleMode(RenderWindow& win, CircleShape& circle, Text& text, float radius, int& cps, bool& left);
void randomCircleMode(RenderWindow& win, CircleShape& circle, Text& text, float radius, int& cps, int& total, int& miss);
void loadResources(Font& font, Texture& background, Music& music);
void resetGame(int& cps, int& total, int& miss, float& timer, Clock& clock, bool& left, CircleShape& circle, float winX, float winY, Vector2f& scale, Vector2f& pos);

int main() {

	srand((int)time(0));

	enum class State {
		MENU,
		GROWING,
		SHRINKING,
		ALTERNATING,
		RANDOM,
		GAME_OVER
	};
	State gameState = State::MENU;

	const string OPTIONS =
		"Press '1' for Growing Mode\n\n"
		"Press '2' for Shrinking Mode\n\n"
		"Press '3' for Alternating Mode\n\n"
		"Press '4' for Random Mode";
	const float GROWING_RADIUS = 45.f;
	const float SHRINKING_RADIUS = 175.f;
	const float ALTERNATING_RADIUS = 100.f;
	const float RANDOM_RADIUS = 95.f;
	
	CircleShape gameCircleObject;
	Vector2f scale = gameCircleObject.getScale();
	Vector2f pos = gameCircleObject.getPosition();
	int cpsValue = 0;
	int totalClickCount = 0;
	int misclickCount = 0;
	float timer = 15.0f;
	float calcCPS;
	float accuracy = 0.f;
	bool displayMisclicks = false;
	bool waitingForLeftClick = true;
	bool isPlayingState = true;
	bool reset = false;
	Clock gameClock;
	float deltaTime;

	VideoMode pixelGraph(Vector2u(1280, 720));
	RenderWindow gameWindow(pixelGraph, "CPS Trainer",
		Style::Close | Style::Resize | Style::Titlebar);
	float windowSizeX = gameWindow.getSize().x;
	float windowSizeY = gameWindow.getSize().y;
	
	Font mainFont;
	Texture background;
	Music backgroundMusic;
	loadResources(mainFont, background, backgroundMusic);
	Sprite backgroundSprite(background);
	backgroundMusic.play();

	// Creating Text objects
	Text titleText(mainFont, "CPS Trainer", 55);
	Text optionsText(mainFont, OPTIONS, 40);
	Text timerText(mainFont, "Timer: ", 25);
	Text cpsText(mainFont, "CPS: 0", 25);
	Text calcCPSText(mainFont, "Average CPS: ", 75);
	Text returnToMenuText(mainFont, "Press ENTER to return to the menu", 90);
	Text randomMisclickText(mainFont, "Misclicks: ", 65);
	Text randomAccuracyText(mainFont, "Accuracy: ", 65);
	vector<Text> textObjects = 
	{ 
		titleText,				// Index 0
		optionsText,			// Index 1
		timerText,				// Index 2
		cpsText,				// Index 3
		calcCPSText,			// Index 4
		returnToMenuText,		// Index 5
		randomMisclickText,		// Index 6
		randomAccuracyText		// Index 7
	};
	initializeTextObjects(textObjects, windowSizeX, windowSizeY);

	

	while (gameWindow.isOpen()) {
		deltaTime = gameClock.restart().asSeconds();

		while (optional event = gameWindow.pollEvent()) {
			if (event->is<Event::Closed>() ||
				(event->is<Event::KeyPressed>() &&
					event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)) {
				gameWindow.close();
			}
			if (event->is<Event::KeyPressed>() && 
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num1) {
				gameState = State::GROWING;
				isPlayingState = true;
			}
			if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num2) {
				gameState = State::SHRINKING;
				isPlayingState = true;
			}
			if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num3) {
				gameState = State::ALTERNATING;
				isPlayingState = true;
			}
			if (event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num4) {
				gameState = State::RANDOM;
				isPlayingState = true;
			}

			// START STATE MACHINE GAME DESIGN
			switch (gameState) {
			case State::GROWING:
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						growingCircleMode(gameWindow, gameCircleObject, textObjects[3], scale, pos, GROWING_RADIUS, cpsValue);
					}
				}
				break;
			case State::SHRINKING:
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						shrinkingCircleMode(gameWindow, gameCircleObject, textObjects[3], scale, pos, SHRINKING_RADIUS, cpsValue);
					}
				}
				break;
			case State::ALTERNATING:
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					if (waitingForLeftClick && mouseEvent->button == Mouse::Button::Left) {
						alternatingCircleMode(gameWindow, gameCircleObject, textObjects[3], ALTERNATING_RADIUS, cpsValue, waitingForLeftClick);
					}
					if (!waitingForLeftClick && mouseEvent->button == Mouse::Button::Right) {
						alternatingCircleMode(gameWindow, gameCircleObject, textObjects[3], ALTERNATING_RADIUS, cpsValue, waitingForLeftClick);
					}
				}
				break;
			case State::RANDOM:
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						randomCircleMode(gameWindow, gameCircleObject, textObjects[3], GROWING_RADIUS, cpsValue, totalClickCount, misclickCount);
						displayMisclicks = true;
					}
				}
				break;
			case State::GAME_OVER:
				if (event->is<Event::KeyPressed>() &&
					event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Enter) {
					gameState = State::MENU;
					reset = true;
					isPlayingState = false;
				}
				break;
			}
		}


		/*
		* UPDATE
		*/
		if (isPlayingState && timer > 0.f) {
			timer -= deltaTime;
			timerText.setString("Time: " + to_string(static_cast<int>(timer)));
			if (timer <= 0.f) {
				isPlayingState = false;
				gameState = State::GAME_OVER;
			}
		}
		if (!isPlayingState && timer <= 0.f) {
			gameClock.restart();
			calcCPS = static_cast<float>(cpsValue) / 15.f;
			textObjects[4].setString("Average CPS: " + to_string(calcCPS).substr(0, 4));
			textObjects[6].setString("Misclicks: " + to_string(misclickCount));
			accuracy = (totalClickCount - static_cast<float>(misclickCount)) / totalClickCount * 100.f;
			textObjects[7].setString("Accuracy: " + to_string(accuracy).substr(0, 4) + "%");
		}
		if (reset) {
			resetGame(cpsValue, totalClickCount, misclickCount, timer, gameClock, waitingForLeftClick, gameCircleObject, windowSizeX, windowSizeY, scale, pos);
		}

		/*
		* RENDER
		*/
		gameWindow.clear();
		gameWindow.draw(backgroundSprite);
		if (gameState == State::MENU) {
			gameWindow.draw(textObjects[0]);
			gameWindow.draw(textObjects[1]);
		}
		else if (isPlayingState) {
			gameWindow.draw(gameCircleObject);
			gameWindow.draw(textObjects[2]);
			gameWindow.draw(textObjects[3]);
		}
		else if (gameState == State::GAME_OVER) {
			gameWindow.draw(textObjects[4]);
			gameWindow.draw(textObjects[5]);
			if (displayMisclicks) {
				gameWindow.draw(textObjects[6]);
				gameWindow.draw(textObjects[7]);
			}
		}
		gameWindow.display();
	}

	return 0;
}

void initializeTextObjects(vector<Text>& textObj, float winX, float winY) {
	// Initializes my Text objects by passing in the Text object and positional coordinates
	
	for (size_t i = 0; i < textObj.size(); i++) {
		FloatRect bounds = textObj[i].getLocalBounds();		// Pulls the size of the gameText object
		textObj[i].setOrigin({
			bounds.position.x + bounds.size.x / 2.f,		// Top left + top right distance / 2 = half
			bounds.position.y + bounds.size.y / 2.f			// Top left + bottom left distance / 2 = half
			});												// Origin is set to the center
		switch (i) {
		case 0:		// Title
			textObj[i].setPosition({ winX / 2.f, winY / 3.f }); break;
		case 1:		// Options
			textObj[i].setPosition({ winX / 2.f, winY / 3.f * 2.f }); break;
		case 2:		// Timer
			textObj[i].setPosition({ winX / 3.f, winY / 10.f }); break;
		case 3:		// CPS
			textObj[i].setPosition({ winX / 3.f * 2.f, winY / 10.f }); break;
		case 4:		// CPS Average
			textObj[i].setPosition({ winX / 2.f, winY / 2.f });
			textObj[i].setFillColor(Color::Black);
			textObj[i].setOutlineColor(Color::White);
			textObj[i].setOutlineThickness(3.5f);
			break;
		case 5:		// Return to Main
			textObj[i].setPosition({ winX / 2.f, winY / 4.f });
			textObj[i].setFillColor(Color::Black);
			textObj[i].setOutlineColor(Color::White);
			textObj[i].setOutlineThickness(3.5f);
			break;
		case 6:		// Misclicks
			textObj[i].setPosition({ winX / 3.f, winY / 4.f * 3.f }); break;
		case 7:		// Accuracy
			textObj[i].setPosition({ winX / 3.f * 2.f, winY / 4.f * 3.f }); break;
	}
	}
}

void growingCircleMode(RenderWindow& win, CircleShape& circle, Text& text, Vector2f& scale, Vector2f& pos, float radius, int& cps) {
	circle.setRadius(radius);
	circle.setOrigin({ radius, radius });
	Vector2f worldPos = win.mapPixelToCoords(Mouse::getPosition(win));
	if (circle.getGlobalBounds().contains(worldPos)) {
		circle.setFillColor(Color::Green);
		scale *= 1.02f;
		circle.setScale(scale);
		circle.setPosition(pos);
		cps++;
		text.setString("CPS: " + to_string(cps));
	}
	else {
		circle.setFillColor(Color::Red);
	}
}

void shrinkingCircleMode(RenderWindow& win, CircleShape& circle, Text& text, Vector2f& scale, Vector2f& pos, float radius, int& cps) {
	circle.setRadius(radius);
	circle.setOrigin({ radius, radius });
	Vector2f worldPos = win.mapPixelToCoords(Mouse::getPosition(win));
	if (circle.getGlobalBounds().contains(worldPos)) {
		circle.setFillColor(Color::Green);
		scale *= 0.98f;
		circle.setScale(scale);
		circle.setPosition(pos);
		cps++;
		text.setString("CPS: " + to_string(cps));
	}
	else {
		circle.setFillColor(Color::Red);
	}
}

void alternatingCircleMode(RenderWindow& win, CircleShape& circle, Text& text, float radius, int& cps, bool& left) {
	circle.setRadius(radius);
	circle.setOrigin({ radius, radius });
	Vector2f worldPos = win.mapPixelToCoords(Mouse::getPosition(win));
	if (left) {
		if (circle.getGlobalBounds().contains(worldPos)) {
			circle.setFillColor(Color::Green);
			left = false;
			cps++;
			text.setString("CPS: " + to_string(cps));
		}
		else {
			circle.setFillColor(Color::Red);
		}
	}
	if (!left) {
		if (circle.getGlobalBounds().contains(worldPos)) {
			circle.setFillColor(Color::Cyan);
			left = true;
			cps++;
			text.setString("CPS: " + to_string(cps));
		}
		else {
			circle.setFillColor(Color::Red);
		}
	}
}

void randomCircleMode(RenderWindow& win, CircleShape& circle, Text& text, float radius, int& cps, int& total, int& miss) {
	int maxXCoord, maxYCoord;
	float newXCoord, newYCoord;
	circle.setRadius(radius);
	circle.setOrigin({ radius, radius });
	Vector2f worldPos = win.mapPixelToCoords(Mouse::getPosition(win));
	if (circle.getGlobalBounds().contains(worldPos)) {
		maxXCoord = static_cast<int>(win.getSize().x - (radius * 2.f));
		maxYCoord = static_cast<int>(win.getSize().y - (radius * 2.f));
		newXCoord = static_cast<float>(rand() % maxXCoord) + radius;
		newYCoord = static_cast<float>(rand() % maxYCoord) + radius;
		circle.setFillColor(Color::Green);
		circle.setPosition({ newXCoord, newYCoord });
		cps++;
		text.setString("CPS: " + to_string(cps));
		total++;
	}
	else {
		circle.setFillColor(Color::Red);
		miss++;
		total++;
	}
}

void loadResources(Font& font, Texture& background, Music& music) {
	if (!font.openFromFile("assets/fonts/Cavalier.ttf")) {
		cout << "[DEBUG] - Font failed to load." << endl;
	}
	if (!background.loadFromFile("assets/graphics/background.jpg")) {
		cout << "[DEBUG] - Background failed to load." << endl;
	}
	if (!music.openFromFile("assets/audio/Infinite Expanse.wav")) {
		cout << "[DEBUG] - Music failed to load." << endl;
	}
	else {
		music.setLooping(true);
		music.setVolume(50.f);
	}
}

void resetGame(int& cps, int& total, int& miss, float& timer, Clock& clock, bool& left, CircleShape& circle, float winX, float winY, Vector2f& scale, Vector2f& pos) {
	cps = 0;
	total = 0;
	miss = 0;
	timer = 15.0f;
	clock.restart();
	left = true;
	circle.setFillColor(Color::White);
	circle.setPosition({ winX / 2.f, winY / 2.f });
	const Vector2f RESET_SCALE = { 1.f, 1.f };
	scale = RESET_SCALE;
	circle.setScale(RESET_SCALE);
}