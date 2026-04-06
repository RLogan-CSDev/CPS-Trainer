#include <Engine.h>
#include <GrowingState.h>		// Must include all game states

Engine::Engine() {
	VideoMode vm(Vector2u(1280, 720));
	m_Window.create(vm, "CPS Trainer",
		Style::Close | Style::Resize || Style::Titlebar,
		State::Windowed);

	if (!m_Font.openFromFile("assets/fonts/Cavalier.ttf")) {
		std::cout << "[DEBUG] Font failed to load.\n";
	}

}

void Engine::setFont(GameState& state) {
	// May be unnecessary, come back later.
}

GameState& Engine::changeState(StateType type) {
	// Resets the game state back to a nullptr
	m_CurrentState.reset();

	switch (type) {
	case StateType::MENU:
		//m_CurrentState = std::make_unique<MenuState>();
		break;
	case StateType::GROWING:
		m_CurrentState = std::make_unique<GrowingState>();
		break;
	case StateType::SHRINKING:
		//m_CurrentState = std::make_unique<ShrinkingState>();
		break;
	case StateType::ALTERNATING:
		//m_CurrentState = std::make_unique<AlternatingState>();
		break;
	case StateType::RANDOM:
		//m_CurrentState = std::make_unique<RandomState>();
		break;
	case StateType::GAME_OVER:
		//m_CurrentState = std::make_unique<GameOverState>();
		break;
	default:
		//m_CurrentState = std::make_unique<MenuState>();
		std::cout << "[DEBUG] State Transition Error!\n";
		break;
	}
	return *m_CurrentState;
}

void Engine::run() {

}