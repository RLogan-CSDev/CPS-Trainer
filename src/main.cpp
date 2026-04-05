#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace sf;

int main() {

	enum class State {
		MENU,
		GROWING,
		SHRINKING,
		ALTERNATING,
		RANDOM,
		GAME_OVER
	};
	State gameState = State::GROWING;							// Game will always start with the Menu

	VideoMode vm(Vector2u(1280, 720));							// Creates a window in a grid format of x, y pixels
	RenderWindow gameWindow(vm, "CPS Trainer",
		Style::Close | Style::Resize | Style::Titlebar);		// Creates the window object

	const float GAME_WINDOW_Y_AXIS_FROM_TOP = gameWindow.getSize().y * -1 + gameWindow.getSize().y;

	srand((int)time(0));

	CircleShape circle;
	circle.setFillColor(Color::Blue);
	circle.setPosition({ gameWindow.getSize().x / 2.f,	// Subtracting radius allows for the circle to truly be centered;
		gameWindow.getSize().y / 2.f });				// without subtracting radius, the points are taken from top-left
														// of the game object.
	Vector2f scale = circle.getScale();
	Vector2f position = circle.getPosition();

	Font font;
	if (!font.openFromFile("assets/fonts/Cavalier.ttf")) {
		cout << "Font failed to load\n";
	}

	int scoreValue = 0;
	float timerValue = 15.0f;
	float cps = 0.0f;

	Text timer(font, "Timer: 15", 25);
	Text scoreText(font, "Score: 0", 25);
	Text avgCPS(font, "Avg: 0", 25);
	timer.setFillColor(Color::White);
	scoreText.setFillColor(Color::Cyan);
	avgCPS.setFillColor(Color::Yellow);
	timer.setPosition({ gameWindow.getSize().x / 3.f, GAME_WINDOW_Y_AXIS_FROM_TOP + 50.f});
	scoreText.setPosition({ gameWindow.getSize().x / 2.f, GAME_WINDOW_Y_AXIS_FROM_TOP / 3.f + 50.f});
	avgCPS.setPosition({ gameWindow.getSize().x / 4.f * 3.f, GAME_WINDOW_Y_AXIS_FROM_TOP + 50.f });

	bool waitingForLeftClick = true;

	Clock gameClock;
	

	while (gameWindow.isOpen()) {
		Time dt = gameClock.restart();
		float deltaTime = dt.asSeconds();
		
		while (optional event = gameWindow.pollEvent()) {
			if (event->is<Event::Closed>() ||
				(event->is<Event::KeyPressed>() &&
				 event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)) {
				gameWindow.close();
			}
			switch (gameState) {
			case State::GROWING: {
				circle.setRadius(50.f);
				float radius = circle.getRadius();
				circle.setOrigin({ radius,radius });
					if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
						if (mouseEvent->button == Mouse::Button::Left) {
							Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
							if (circle.getGlobalBounds().contains(worldPos)) {
								circle.setFillColor(Color::White);
								scale *= 1.05f;
								circle.setScale(scale);
								circle.setPosition(position);

								scoreValue++;
								scoreText.setString("Score: " + to_string(scoreValue));
							}
							else {
								circle.setFillColor(Color::Red);
							}
						}
					}
				break;
			}
			case State::SHRINKING: {
				circle.setRadius(200.f);
				float radius = circle.getRadius();
				circle.setOrigin({ radius, radius });
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					if (mouseEvent->button == Mouse::Button::Left) {
						Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
						if (circle.getGlobalBounds().contains(worldPos)) {
							circle.setFillColor(Color::Magenta);
							scale *= 0.95f;
							circle.setScale(scale);
							Vector2f position = circle.getPosition();
							circle.setPosition(position);

							scoreValue++;
							scoreText.setString("Score: " + to_string(scoreValue));
						}
						else {
							circle.setFillColor(Color::Red);
						}
					}
				}
				break;
			}
			case State::ALTERNATING: {
				circle.setRadius(100.f);
				float radius = circle.getRadius();
				circle.setOrigin({ radius, radius });
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
					if (waitingForLeftClick && mouseEvent->button == Mouse::Button::Left && mouseEvent->button != Mouse::Button::Right) {
						if (circle.getGlobalBounds().contains(worldPos)) {
							circle.setFillColor(Color::Yellow);

							waitingForLeftClick = false;

							scoreValue++;
							scoreText.setString("Score: " + to_string(scoreValue));
						}
						else {
							circle.setFillColor(Color::Red);
						}
					}
					if (!waitingForLeftClick && mouseEvent->button == Mouse::Button::Right && mouseEvent->button != Mouse::Button::Left) {
						if (circle.getGlobalBounds().contains(worldPos)) {
							circle.setFillColor(Color::Green);

							waitingForLeftClick = true;

							scoreValue++;
							scoreText.setString("Score: " + to_string(scoreValue));
						}
						else {
							circle.setFillColor(Color::Red);
						}
					}
				}
				break;
			}
			case State::RANDOM: {
				circle.setRadius(75.f);
				float radius = circle.getRadius();
				circle.setOrigin({ radius, radius });
				if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
					Vector2f worldPos = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
					if (mouseEvent->button == Mouse::Button::Left) {
						if (circle.getGlobalBounds().contains(worldPos)) {
							int maxXCoord = static_cast<int>(gameWindow.getSize().x - (radius * 2.f));
							int maxYCoord = static_cast<int>(gameWindow.getSize().y - (radius * 2.f));
							float newXCoord = static_cast<float>(rand() % maxXCoord) + radius;
							float newYCoord = static_cast<float>(rand() % maxYCoord) + radius;
							circle.setPosition({ newXCoord, newYCoord });
							circle.setFillColor(Color::Blue);

							scoreValue++;
							scoreText.setString("Score: " + to_string(scoreValue));
						}
						else {
							circle.setFillColor(Color::Red);
						}
					}
				}
				break;
			}
			}
		}
				
		if ((gameState == State::GROWING || gameState == State::SHRINKING 
			|| gameState == State::ALTERNATING || gameState == State::RANDOM) && timerValue > 0.0f) {
			timerValue -= deltaTime;
			timer.setString("Time: " + to_string(static_cast<int>(timerValue)));
		}
		else {
			float calcCPS = static_cast<float>(scoreValue) / 15.0f;
			avgCPS.setString("Avg: " + to_string(calcCPS).substr(0, 4));
			
			timerValue = 0.0f;
			gameState = State::GAME_OVER;
		}

		gameWindow.clear();
		gameWindow.draw(circle);
		gameWindow.draw(timer);
		gameWindow.draw(scoreText);
		gameWindow.draw(avgCPS);
		gameWindow.display();
	}

	return 0;
}
