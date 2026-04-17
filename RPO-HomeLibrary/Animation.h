#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// ---------------Класс анимации---------------
class Animation // У анимации есть: несколько текстур (кадров), текущий кадр, скорость смены кадров, общая длительность (используется для условия проигрывания анимации), таймеры
{
public:
	Animation() {}
	Animation(int currentFrame, float frameDelay, float totalDuration);

	// Функция по загрузке файлов кадров, принимающая массив строк, ссылающихся на пути до кадров
	bool LoadFrames(const std::vector<std::string>& filePaths);
	bool GetIsActive() const
	{
		return isActive;
	}
	// Функция, выставляющая значения в изначальное положение
	void StartAnimation();
	// Функция, производящая продвижение по кадрам
	bool UpdateAnimation();
	// Функция отрисовки анимации в графическое окно
	void Draw(sf::RenderWindow& window);
private:
	std::vector<std::shared_ptr<sf::Texture>> frames; // массив умных указателей на текстуры
	int currentFrame = 0;
	bool isActive = false;
	float frameDelay = 0.1f; // скорость смены (задержки) кадров
	float totalDuration = 1.4f;
	sf::Clock frameClock; // таймер смены кадра
	sf::Clock totalClock; // таймер учета всего прошедшенго времени
};