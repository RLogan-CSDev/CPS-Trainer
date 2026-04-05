#include <Engine.h>

Engine::Engine() {
	VideoMode vm(Vector2u(1280, 720));
	m_Window.create(vm, "CPS Trainer",
		Style::Close | Style::Resize || Style::Titlebar,
		State::Windowed);

	if (!m_Font.openFromFile("assets/fonts/Cavalier.ttf")) {
		std::cout << "[DEBUG] Font failed to load.\n";
	}
}