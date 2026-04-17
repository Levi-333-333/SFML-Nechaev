#pragma once
#include <SFML/Graphics.hpp>

// ---------------Класс поля ввода---------------
// shape, событие нажатия по фигуре, отрисовка букв, положение, размер, get-функция по возвращению значения объекта, подпись к полю ввода
class InputField 
{
public:
	// Удобно разграничить функционал конструктора следующим образом: инициализацию производить в значениях по умолчанию, а настраивать объекты в теле конструктора
	//			шрифт,				 подпись к полю ввода,		позиция в окне,		размер
	InputField(const sf::Font& font, sf::String caption, sf::Vector2f position, sf::Vector2f size);
	// Функция для проверки попадания в окно
	// Если точка, передаваемая в качестве параметра лежит в границах окна, возвращаем true
	bool Contains(sf::Vector2f point) const;
	// Функция изменения цвета фигуры при активном или неактивном режиме 
	void SetActive(bool isActive);
	// Get-функция по возвращению строкового значения поля ввода
	const sf::String& GetValue() const
	{
		return valueString;
	}
	// Get-функция по возвращению значения подписи поля ввода
	sf::Text GetCaptionText() const
	{
		return captionText;
	}
	// Вспомогательная функция очистки текста в поле ввода
	void ClearInputValue()
	{
		valueString.clear();
	}
	// Функция-обработчик события введения символа пользователем, принимающая символ юникода
	// char_32_t - тип данных, захватывающий в два больше символов ASCII, где располагается кириллица
	void HandleTextEntered(char32_t unicode);
	// Функция отрисовки, принимающая ссылку на графическое окно
	void Draw(sf::RenderWindow& renderWindow);
private:
	// Отрисовываемые элементы
	sf::RectangleShape fieldShape;
	sf::Text captionText; // подпись
	sf::Text valueText;
	// Прочие элементы
	sf::String valueString;
	bool isActive = false;
	unsigned int maxLength = 32;
};