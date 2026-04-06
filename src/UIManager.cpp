#include <UIManager.h>

UIManager::UIManager(Font& font, RenderWindow& window) {
	Vector2u windowSize = window.getSize();
	m_HUD.setSize({ windowSize.x, windowSize.y });
	m_HUD.setCenter({ windowSize.x / 2.f, windowSize.y / 2.f });
	m_HUD.setViewport(FloatRect({ 0.f,0.f }, { 1.f,1.f }));

	Text timerText(font, "", 30);
	timerText.setFillColor(Color::White);
	FloatRect timerRect = timerText.getLocalBounds();
	timerText.setOrigin(timerRect.getCenter());
	timerText.setPosition({ windowSize.x / 4.f, windowSize.y * 0.1f });


	Text cpsText(font, "", 30);
	cpsText.setFillColor(Color::White);
	FloatRect cpsRect = cpsText.getLocalBounds();
	cpsText.setOrigin(cpsRect.getCenter());
	cpsText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.1f });

	Text calcCPSText(font, "", 30);
	calcCPSText.setFillColor(Color::White);
	FloatRect calcCPSRect = calcCPSText.getLocalBounds();
	calcCPSText.setOrigin(calcCPSRect.getCenter());
	calcCPSText.setPosition({ windowSize.x / 4.f * 3.f, windowSize.y / 2.f });
}

void UIManager::drawUI() {

}