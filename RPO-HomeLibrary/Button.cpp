#include "Button.h"
#include "colors.h"

Button::Button(const sf::Font& font, sf::String label, sf::Vector2f position, sf::Vector2f size) : label(font, label, 18)
{
	buttonShape.setPosition(position);
	buttonShape.setSize(size);
	buttonShape.setFillColor(sf::Color(EGGSHELL_COLOR));
	buttonShape.setOutlineColor(CHARCOAL_BROWN_COLOR);
	buttonShape.setOutlineThickness(3.0f);

	this->label.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
	this->label.setPosition({ position.x + 10.0f, position.y + 5.0f });
}

bool Button::Contains(sf::Vector2f point) const
{
	const sf::Vector2f position = buttonShape.getPosition();
	const sf::Vector2f size = buttonShape.getSize();

	return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(buttonShape);
	window.draw(label);
}