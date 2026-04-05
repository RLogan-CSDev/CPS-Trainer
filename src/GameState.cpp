#include <GameState.h>

void GameState::handleInput(RenderWindow& window) {	
	while (std::optional event = window.pollEvent()) {
		if (event->is<Event::Closed>() ||
			(event->is<Event::KeyPressed>() &&
				event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)) {
			// Game closes via "X" button or "Esc" key
			window.close();
		}

		if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>()) {
			if (mouseEvent->button == Mouse::Button::Left) {
				// Uses a "default" Left-click method for different games modes.
				// Override in necessary classes as applicable for different keys/buttons pressed.

			}
		}
	}
}