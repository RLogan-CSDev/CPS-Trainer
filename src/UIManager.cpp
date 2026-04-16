#include <UIManager.h>

UIManager::UIManager(Font& font, RenderWindow& window) {
	m_Font = font;
	Vector2u windowSize = window.getSize();
	//m_HUD.setSize({ windowSize.x, windowSize.y });
	m_HUD.setCenter({ windowSize.x / 2.f, windowSize.y / 2.f });
	m_HUD.setViewport(FloatRect({ 0.f,0.f }, { 1.f,1.f }));

	Text timerText(m_Font, "", 30);
	timerText.setFillColor(Color::White);
	FloatRect timerRect = timerText.getLocalBounds();
	timerText.setOrigin(timerRect.getCenter());
	timerText.setPosition({ windowSize.x / 4.f, windowSize.y * 0.1f });
	m_GameplayElements.push_back(timerText);


	Text cpsText(m_Font, "", 30);
	cpsText.setFillColor(Color::White);
	FloatRect cpsRect = cpsText.getLocalBounds();
	cpsText.setOrigin(cpsRect.getCenter());
	cpsText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.1f });
	m_GameplayElements.push_back(cpsText);

	Text calcCPSText(m_Font, "", 30);
	calcCPSText.setFillColor(Color::White);
	FloatRect calcCPSRect = calcCPSText.getLocalBounds();
	calcCPSText.setOrigin(calcCPSRect.getCenter());
	calcCPSText.setPosition({ windowSize.x / 4.f * 3.f, windowSize.y / 2.f });
	m_GameplayElements.push_back(calcCPSText);
}

void UIManager::drawUI(RenderWindow& window, bool isMenu) {
	const View& oldView = window.getView();
	window.setView(m_HUD);

	if (isMenu) {
		for (const auto& text : m_MenuElements) {
			window.draw(text);
		}
	}
	else {
		for (const auto& text : m_GameplayElements) {
			window.draw(text);
		}
	}
	
	window.setView(oldView);
}

void UIManager::createMainMenu(RenderWindow& window) {
	m_MenuElements.clear();

	// Title Screen
	Text titleText(m_Font, "CPS Trainer", 55);
	titleText.setPosition({ 200, 250 });
	m_MenuElements.push_back(titleText);

	Text optionsText(m_Font, "Press '1' for Growing Mode\n\n"
		"Press '2' for Shrinking Mode\n\n"
		"Press '3' for Alternating Mode\n\n"
		"Press '4' for Random Mode", 40);
	optionsText.setPosition({ 200, 345 });
	m_MenuElements.push_back(optionsText);
}