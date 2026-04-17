#pragma once
#include <SFML/Graphics.hpp>

// ---------------Класс кнопки---------------
class Button
{
public:
	Button(const sf::Font& font, sf::String label, sf::Vector2f position, sf::Vector2f size);
	
	bool Contains(sf::Vector2f point) const;
	void Draw(sf::RenderWindow& window);

	sf::RectangleShape buttonShape;
private:
	sf::Text label;
};