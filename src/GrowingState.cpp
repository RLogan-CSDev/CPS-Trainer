#include <GrowingState.h>

void GrowingState::update(float elapsedTime) {
	if (m_TimerValue > 0.0f) {
		m_TimerValue -= elapsedTime;
		m_Scale *= std::pow(m_GrowthRate, elapsedTime);
		m_Circle.setScale(m_Scale);
		m_Circle.setFillColor(Color::Blue);
	}
	else {
		m_TimerValue = 0.0f;
		m_Circle.setFillColor(Color::White);
	}
}

void GrowingState::draw() {

}

GrowingState::GrowingState(RenderWindow& window) {
	Vector2f center;
	center.x = window.getSize().x / 2.0f;
	center.y = window.getSize().y / 2.0f;

	m_Radius = 50.f;
	m_GrowthRate = 1.05f;
	m_TimerValue = 15.0f;

	m_Circle.setFillColor(Color::White);
	m_Circle.setPosition({ center.x, center.y });
	m_Circle.setRadius(m_Radius);
	m_Circle.setOrigin({ m_Radius, m_Radius });

	m_Scale = m_Circle.getScale();
}

GrowingState::~GrowingState() {
	std::cout << "[DEBUG] GrowingState cleaned up successfully.\n";
}
