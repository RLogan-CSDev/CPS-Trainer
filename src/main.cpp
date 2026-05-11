#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <string>

using namespace std;
using namespace sf;

/*
* THIS IS THE FINAL VERSION OF THE CPS TRAINER
* 
* The purpose of this game is so that the player can train their
* clicks-per-second on a standard mouse using Left and Right clicks.
* 
* There are 4 Game Modes to play:
* (1) Growing - each successive left-click grows the scale of the circle
* (2) Shrinking - each successive left-click shrinks the scale of the circle
* (3) Alternating - each successive left-click must be followed up by a right-click which
*					then must be followed up by a left-click.
* (4) Random - the circle spawns in random locations on each successive left-click, allowing
*				for precision and accuracy training.
* 
* The game is scalable and can easily implement new game modes as desired (Ctrl+F "GAME MODES HERE")
*/

int main() {

	// 1. Create a window for the game.
	VideoMode pixelGraph(Vector2u(1280, 720));
	RenderWindow gameWindow(pixelGraph, "CPS Trainer",
		Style::Close | Style::Resize | Style::Titlebar);
	float windowSizeX = gameWindow.getSize().x;				// Variable for using horizontal window size
	float windowSizeY = gameWindow.getSize().y;				// Variable for using vertical window size

	// 2. Create the core game object, a circle!
	CircleShape gameCircle;
	gameCircle.setFillColor(Color::White);					// White = default
	gameCircle.setPosition({ windowSizeX / 2.f,	
		windowSizeY / 2.f });								// Sets circle position in center of screen
	float growingRadius = 45.f;								// Radius for growing mode
	float shrinkingRadius = 175.f;							// Radius for shrinking mode
	float alternatingRadius = 100.f;						// Radius for alternating mode
	float randomRadius = 95.f;								// Radius for random mode
	const float RESET_RADIUS = 50.f;
	const Vector2f RESET_SCALE = { 1.f , 1.f };
	Vector2f debugScale;									// Debugger to see how the scale changes as game state changes

	// 3. Load the font.
	Font mainFont;
	if (!mainFont.openFromFile("assets/fonts/Cavalier.ttf")) {
		// If the font does not load from the files, an error message prints to console.
		cout << "[DEBUG] - Font failed to load.\n";
	}
	// 3. Load the background.
	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("assets/graphics/background.jpg")) {
		// If the graphic does not load from the files, an error message prints to console.
		cout << "[DEBUG] - Background failed to load.\n";
	}
	Sprite backgroundSprite(backgroundTexture);
	// 3. Load the background music.
	Music backgroundMusic;
	if (!backgroundMusic.openFromFile("assets/audio/Infinite Expanse.wav")); {
		// If the background music does not load from the files, an error message prints to console.
		cout << "[DEBUG] - Music failed to load.\n";
	}
	backgroundMusic.setLooping(true);
	backgroundMusic.setVolume(50.f);
	backgroundMusic.play();

	// 4. Create Text objects.
	// 1 - Title Text
	Text titleText(mainFont, "CPS Trainer", 55);			// Title of game for the menu
	FloatRect titleBounds = titleText.getLocalBounds();		// Pulls the size of the title text object
	titleText.setOrigin({
		titleBounds.position.x + titleBounds.size.x / 2.f,	// Top left + top right distance / 2 = half
		titleBounds.position.y + titleBounds.size.y / 2.f	// Top left + bottom left distance / 2 = half
		});													// Origin is set to the center
	titleText.setPosition({ windowSizeX / 2.f,
		windowSizeY / 3.f });								// Places title in horizontal middle, 1/3rd from top
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
	timerText.setPosition({ windowSizeX / 3.f,
		windowSizeY / 10.f });								// Places timer 1/3rd horizontal distance from left, 1/10th from top
	// 4 - CPS Text
	Text cpsText(mainFont, "CPS: 0", 25);
	FloatRect cpsBounds = cpsText.getLocalBounds();
	cpsText.setOrigin({
		cpsBounds.position.x + cpsBounds.size.x / 2.f,
		cpsBounds.position.y + cpsBounds.size.y / 2.f
		});
	cpsText.setPosition({ windowSizeX / 3.f * 2.f,
		windowSizeY / 10.f });								// Places CPS 2/3rds horizontally distance from left, 1/10th from top
	// 5 - Average Calculation Text
	Text calcCPSText(mainFont, "Average CPS: ", 75);
	FloatRect calcCPSBounds = calcCPSText.getLocalBounds();
	calcCPSText.setOrigin({
		calcCPSBounds.position.x + calcCPSBounds.size.x / 2.f,
		calcCPSBounds.position.y + calcCPSBounds.size.y / 2.f
		});
	calcCPSText.setPosition({ windowSizeX / 2.f,
		windowSizeY / 2.f });								// Places average calculation in exact center of screen
	calcCPSText.setFillColor(Color::Black);					// Sets color of this text to black
	calcCPSText.setOutlineColor(Color::White);				// Sets outline color
	calcCPSText.setOutlineThickness(5.f);					// Sets thickness of outline to 5 pixels thick
	// 6 - Return to Main Text
	Text returnToMenuText(mainFont, "Press ENTER to return to menu", 95);
	FloatRect returnToMenuBounds = returnToMenuText.getLocalBounds();
	returnToMenuText.setOrigin({
		returnToMenuBounds.position.x + returnToMenuBounds.size.x / 2.f,
		returnToMenuBounds.position.y + returnToMenuBounds.size.y / 2.f
		});
	returnToMenuText.setPosition({ windowSizeX / 2.f,
		windowSizeY / 4.f });
	returnToMenuText.setFillColor(Color::Black);
	returnToMenuText.setOutlineColor(Color::White);
	returnToMenuText.setOutlineThickness(2.f);
	// 7 - Alternative Random Statistics Text
	Text randomMisclickText(mainFont, "Misclicks: ", 65);
	FloatRect randomMisclickBounds = randomMisclickText.getLocalBounds();
	randomMisclickText.setOrigin({
		randomMisclickBounds.position.x + randomMisclickBounds.size.x / 2.f,
		randomMisclickBounds.position.y + randomMisclickBounds.size.y / 2.f
		});
	randomMisclickText.setPosition({ windowSizeX / 3.f,				// Places horizontal position 1/3 from left
		windowSizeY / 4.f * 3.f });									// Places vertical position 3/4s from the top
	Text randomAccuracyText(mainFont, "Accuracy: ", 65);
	FloatRect randomAccuracyBounds = randomAccuracyText.getLocalBounds();
	randomAccuracyText.setOrigin({
		randomAccuracyBounds.position.x + randomAccuracyBounds.size.x / 2.f,
		randomAccuracyBounds.position.y + randomAccuracyBounds.size.y / 2.f
		});
	randomAccuracyText.setPosition({ windowSizeX / 3.f * 2.f,		// Places horizontal position 2/3s from left
		windowSizeY / 4.f * 3.f });									// Places vertical position 3/4s from top

	// 5. Create State Machine - GAME MODES HERE
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
	int cpsValue = 0;										// Collecting click count
	float timerValue = 15.f;								// Timer starts at 15 seconds
	float calcCPS;											// Calculates average CPS
	bool waitingForLeftClick = true;						// Needed for ALTERNATING state
	srand((int)time(0));									// Needed for RANDOM state
	Clock gameClock;
	Time dt = gameClock.restart();
	float deltaTime = dt.asSeconds();
	Vector2f circleScale = gameCircle.getScale();			// Retrieves current scale of circle
	Vector2f circlePosition = gameCircle.getPosition();		// Retrieves circles position assigned above
	bool isPlayingState = false;							// Controls game logic based on game state machine
	bool resetGame = false;									// Controls the game reset logic
	int misclickCount = 0;									// Used certain game modes to track another statistic
	float randomAccuracyCount = 0.f;						// Calculates accuracy based on total clicks
	float totalClicks = 0.f;								// Tracks total clicks needed for math later
	bool displayMisclick = false;							// Needed for controlling whether to display misclicks

	// 7. Game Loop
	while (gameWindow.isOpen()) {
		deltaTime = gameClock.restart().asSeconds();		// Resets clock each frame
		/*
		* INPUT - Capture keyboard/mouse/window interactions
		*/
		while (optional currentEvent = gameWindow.pollEvent()) {
			if (currentEvent->is<Event::Closed>() ||
				(currentEvent->is<Event::KeyPressed>() &&
					currentEvent->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)) {
				gameWindow.close();
			}
			// Separate if structures since game state changes based on input vs. window interaction
			if (currentEvent->is<Event::KeyPressed>() &&
				currentEvent->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num1) {
				// Switch to playing state - GROWING
				gameState = State::GROWING;
				isPlayingState = true;
			}
			else if (currentEvent->is<Event::KeyPressed>() &&
				currentEvent->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num2) {
				// Switch to playing state - SHRINKING
				gameState = State::SHRINKING;
				isPlayingState = true;
			}
			else if (currentEvent->is<Event::KeyPressed>() &&
				currentEvent->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num3) {
				// Switch to playing state - ALTERNATING
				gameState = State::ALTERNATING;
				isPlayingState = true;
			}
			else if (currentEvent->is<Event::KeyPressed>() &&
				currentEvent->getIf<Event::KeyPressed>()->code == Keyboard::Key::Num4) {
				// Switch to playing state - RANDOM
				gameState = State::RANDOM;
				isPlayingState = true;
			}
			switch (gameState) {
			case State::GROWING: 
				resetGame = false;
				displayMisclick = false;
				gameCircle.setRadius(growingRadius);
				gameCircle.setOrigin({ gameCircle.getRadius(), gameCircle.getRadius() });
				if (const auto* mouseEvent = currentEvent->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						// Detects left click
						Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
						// Allows for the mouse movements to be detected
						if (gameCircle.getGlobalBounds().contains(worldPos)) {
							// Only when the player clicks on the circle
							gameCircle.setFillColor(Color::Green);
							circleScale *= 1.03f;						// Increase in size by about 2% exponentially
							gameCircle.setScale(circleScale);
							gameCircle.setPosition(circlePosition);
							cpsValue++;									// Collect each click
							cpsText.setString("CPS: " + to_string(cpsValue));
							debugScale = gameCircle.getScale();
							cout << "[DEBUG] - Circle current scale: " << debugScale.x << " & " << debugScale.y << endl;
						}
						else {
							gameCircle.setFillColor(Color::Red);
						}
					}
				}
				break;
			case State::SHRINKING:
				resetGame = false;
				displayMisclick = false;
				gameCircle.setRadius(shrinkingRadius);
				gameCircle.setOrigin({ gameCircle.getRadius(), gameCircle.getRadius() });
				if (const auto* mouseEvent = currentEvent->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						// Detects left click
						Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
						// Allows for the mouse movements to be detected
						if (gameCircle.getGlobalBounds().contains(worldPos)) {
							// Only when the player clicks on the circle
							gameCircle.setFillColor(Color::Green);
							circleScale *= 0.97f;						// Decrease in size by about 2% exponentially
							gameCircle.setScale(circleScale);
							gameCircle.setPosition(circlePosition);
							cpsValue++;									// Collect each click
							cpsText.setString("CPS: " + to_string(cpsValue));
							debugScale = gameCircle.getScale();
							cout << "[DEBUG] - Circle current scale: " << debugScale.x << " & " << debugScale.y << endl;
						}
						else {
							gameCircle.setFillColor(Color::Red);
						}
					}
				}
				break;
			case State::ALTERNATING:
				resetGame = false;
				displayMisclick = false;
				gameCircle.setRadius(alternatingRadius);
				gameCircle.setOrigin({ gameCircle.getRadius(), gameCircle.getRadius() });
				if (const auto* mouseEvent = currentEvent->getIf<Event::MouseButtonPressed>()) {
					Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
					// Strictly accepts left click only
					if (waitingForLeftClick && mouseEvent->button == Mouse::Button::Left &&
						mouseEvent->button != Mouse::Button::Right) {
						// Allows for the mouse movements to be detected
						if (gameCircle.getGlobalBounds().contains(worldPos)) {
							// Only when the player clicks on the circle
							gameCircle.setFillColor(Color::Green);
							waitingForLeftClick = false;				// Change to accept right click
							cpsValue++;									// Collect click
							cpsText.setString("CPS: " + to_string(cpsValue));
							debugScale = gameCircle.getScale();
							cout << "[DEBUG] - Circle current scale: " << debugScale.x << " & " << debugScale.y << endl;
						}
						else {
							gameCircle.setFillColor(Color::Red);
						}
					}
					// Strictly accepts right click only
					if (!waitingForLeftClick && mouseEvent->button == Mouse::Button::Right &&
						mouseEvent->button != Mouse::Button::Left) {
						// Allows for the mouse movements to be detected
						if (gameCircle.getGlobalBounds().contains(worldPos)) {
							// Only when the player clicks on the circle
							gameCircle.setFillColor(Color::Cyan);
							waitingForLeftClick = true;					// Change to accept left click
							cpsValue++;									// Collect click
							cpsText.setString("CPS: " + to_string(cpsValue));
							debugScale = gameCircle.getScale();
							cout << "[DEBUG] - Circle current scale: " << debugScale.x << " & " << debugScale.y << endl;
						}
						else {
							gameCircle.setFillColor(Color::Red);
						}
					}
				}
				break;
			case State::RANDOM:
				resetGame = false;
				gameCircle.setRadius(randomRadius);
				gameCircle.setOrigin({ gameCircle.getRadius(), gameCircle.getRadius() });
				if (const auto* mouseEvent = currentEvent->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						// Detects left click
						Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
						// Allows for the mouse movements to be detected
						if (gameCircle.getGlobalBounds().contains(worldPos)) {
							// Only when the player clicks on the circle
							int maxXCoord = static_cast<int>(windowSizeX - (gameCircle.getRadius() * 2.f));			// Create a maximum horizontal spawn distance
							int maxYCoord = static_cast<int>(windowSizeY - (gameCircle.getRadius() * 2.f));			// Create a maximum vertical spawn distance
							float newXCoord = static_cast<float>(rand() % maxXCoord) + gameCircle.getRadius();		// Randomly assigns a new horizontal position
							float newYCoord = static_cast<float>(rand() % maxYCoord) + gameCircle.getRadius();		// Randomly assigns a new vertical position
							gameCircle.setFillColor(Color::Green);
							gameCircle.setPosition({ newXCoord, newYCoord });			// Sets random position
							cpsValue++;													// Collect click
							cpsText.setString("CPS: " + to_string(cpsValue));
							totalClicks++;
						}
						else {
							gameCircle.setFillColor(Color::Red);
							misclickCount++;
							displayMisclick = true;
							totalClicks++;
						}
					}
				}
				break;
			case State::GAME_OVER:
				if (currentEvent->is<Event::KeyPressed>() &&
					currentEvent->getIf<Event::KeyPressed>()->code == Keyboard::Key::Enter) {
					gameState = State::MENU;
					resetGame = true;									// Triggers everything else to reset
					gameCircle.setRadius(RESET_RADIUS);					// Sets circle radius back to initial value
				}
			}
		}


		/*
		* UPDATE - Game Logic
		*/
		if (isPlayingState && timerValue > 0.0f) {
			timerValue -= deltaTime;
			timerText.setString("Time: " 
				+ to_string(static_cast<int>(timerValue)));
			if (timerValue <= 0.0f) {
				isPlayingState = false;									// Game has ended once timer is finished
				gameState = State::GAME_OVER;
			}
		}
		if (!isPlayingState && timerValue <= 0.0f) {
			gameClock.restart();
			calcCPS = static_cast<float>(cpsValue) / 15.0f;				// Calculate average AFTER game is over
			calcCPSText.setString("Average CPS: " +
				to_string(calcCPS).substr(0, 4));						// .substr function keeps average set to two decimal places

			randomMisclickText.setString("Misclicks: " 
				+ to_string(misclickCount));
			randomAccuracyCount = 
				(totalClicks - static_cast<float>(misclickCount)) / totalClicks * 100.f;
			randomAccuracyText.setString("Accuracy: " 
				+ to_string(randomAccuracyCount).substr(0,4) + "%");	// Calculates how many clicks registered vs. those that did not register
		}
		if (resetGame) {
			cpsValue = 0;
			timerValue = 15.0f;
			misclickCount = 0;
			randomAccuracyCount = 0.f;
			totalClicks = 0.f;
			gameClock.restart();
			gameCircle.setFillColor(Color::White);						// White = default
			gameCircle.setPosition({ windowSizeX / 2.f,
				windowSizeY / 2.f });									// Sets circle position in center of screen
			circleScale = RESET_SCALE;									// Allows for the scale to go back to normal
			gameCircle.setScale(RESET_SCALE);							// Resets size of circle in subsequent played games
			debugScale = gameCircle.getScale();
			cout << "[DEBUG] - Before game reset : " 
				<< debugScale.x << " & " << debugScale.y << endl;
			cpsText.setString("CPS: 0");
			waitingForLeftClick = true;
		}

		/*
		* RENDER - Clears screen and draws objects
		*/
		gameWindow.clear();
		gameWindow.draw(backgroundSprite);
		if (gameState == State::MENU) {
			// Draw only menu
			gameWindow.draw(titleText);
			gameWindow.draw(optionsText);
		}
		if (isPlayingState) {
			// Draw appropriate game mode
			gameWindow.draw(gameCircle);
			gameWindow.draw(timerText);
			gameWindow.draw(cpsText);
		}
		if (gameState == State::GAME_OVER) {
			// Draw end of game stats
			gameWindow.draw(calcCPSText);
			gameWindow.draw(returnToMenuText);
			if (displayMisclick) {
				gameWindow.draw(randomMisclickText);
				gameWindow.draw(randomAccuracyText);
			}
		}
		gameWindow.display();
	}
	return 0;
}