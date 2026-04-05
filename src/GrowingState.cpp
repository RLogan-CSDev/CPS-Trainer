#include <GrowingState.h>

void GrowingState::init(RenderWindow& window) {
	Vector2f center;
	center.x = window.getSize().x / 2.0f;
	center.y = window.getSize().y / 2.0f;
	
	float radius = 50.f;

	m_Circle.setFillColor(Color::White);
	m_Circle.setPosition({ center.x, center.y });
	m_Circle.setRadius(radius);
	m_Circle.setOrigin({ radius,radius });
	
	m_Scale = m_Circle.getScale();

	// Set up Font reference here from Engine.

	//Text timer(font);
}

void GrowingState::update(float elapsedTime) {
	float growthRate = 1.05f;
	if (m_TimerValue > 0.0f) {
		m_TimerValue -= elapsedTime;
		m_Scale *= std::pow(growthRate, elapsedTime);
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

GrowingState::~GrowingState() {
	std::cout << "[DEBUG] GrowingState cleaned up successfully.\n";
}