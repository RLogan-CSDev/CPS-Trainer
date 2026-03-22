#include <SFML/Graphics.hpp>

using namespace sf;

int main() {
	RenderWindow window(VideoMode({ 800,600 }), "CPS Trainer - Test");

	CircleShape shape(50.f);
	shape.setFillColor(Color::Green);
	shape.setPosition({ 375.f,275.f });

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<Event::Closed>()) {
				window.close();
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	return 0;
}