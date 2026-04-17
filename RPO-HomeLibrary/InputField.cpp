#include "InputField.h"
#include "colors.h"

InputField::InputField(const sf::Font& font, sf::String caption, sf::Vector2f position, sf::Vector2f size) : captionText(font, caption, 18), valueText(font, L"Я пример", 18)
{

	// fieldShape
	fieldShape.setPosition(position);
	fieldShape.setSize(size);
	fieldShape.setFillColor(sf::Color(EGGSHELL_COLOR));
	fieldShape.setOutlineColor(sf::Color(CHARCOAL_BROWN_COLOR));
	fieldShape.setOutlineThickness(3.0f);
	// caption
	captionText.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
	captionText.setPosition({ position.x, position.y - 25.0f });
	// valueText
	valueText.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
	valueText.setPosition({ position.x + 10.0f, position.y + 5.0f });
}

bool InputField::Contains(sf::Vector2f point) const
{
	const sf::Vector2f position = fieldShape.getPosition();
	const sf::Vector2f size = fieldShape.getSize();

	return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}

void InputField::SetActive(bool isActive)
{
	this->isActive = isActive;
	fieldShape.setOutlineColor(isActive ? sf::Color(66, 62, 55, 255) : sf::Color(66, 62, 55, 150));
}

void InputField::HandleTextEntered(char32_t unicode)
{
	if (!isActive) return; // Если пользователь не в активном окне ввода, то завершаем работу функции
	// Ситуации стирания
	if (unicode == 8) // Backspace
	{
		if (!valueString.isEmpty()) valueString.erase(valueString.getSize() - 1);
	}
	else if (unicode >= 32) // Ситуация добавления символов
	{
		if (valueString.getSize() < maxLength) valueString += sf::String(unicode);
	}
}

void InputField::Draw(sf::RenderWindow& renderWindow)
{
	valueText.setString(valueString);
	renderWindow.draw(captionText);
	renderWindow.draw(fieldShape);
	renderWindow.draw(valueText);
}