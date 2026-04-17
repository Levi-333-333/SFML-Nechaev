#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Animation::Animation(int currentFrame, float frameDelay, float totalDuration)
{
	this->currentFrame = currentFrame;
	this->frameDelay = frameDelay;
	this->totalDuration = totalDuration;
}

bool Animation::LoadFrames(const std::vector<std::string>& filePaths)
{
	// Первым делом имеющиеся загруженные кадры в объект очищаются, для того чтобы можно было повторно использовать объект класса Animation. Это выгоднее чем создание полностью нового объекта, потому что не выделяется новая память под объект.
	frames.clear();
	// После полной очистки массива становится неизвестно количество объектов. Для этого заранее резервируем память под количество поступающих объектов в параметр функции.
	frames.reserve(filePaths.size());

	for (const std::string& filePath : filePaths)
	{
		// Создаем объект умного указателя на текстуру
		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
		// Пытаемся загрузить в объект текстуры файл
		if (!texture->loadFromFile(filePath))
		{
			std::cerr << "Файл текстуры не найден: " << filePath << std::endl;
			return false;
		}
		frames.push_back(texture);
	}

	return !frames.empty();
}

void Animation::StartAnimation()
{
	isActive = true;
	frameClock.restart();
	totalClock.restart();
	currentFrame = 0;
}

bool Animation::UpdateAnimation()
{
	// Если анимация не активна, то возвращаем false
	if (!isActive) return false;

	// Опиарясь на таймер смены кадра, если он становится больше, чем время задержки (скорости смены кадра), то прибавляем к текущему кадру + 1
	if (frameClock.getElapsedTime().asSeconds() > frameDelay)
	{
		// При простом увеличении кадра рано или поздно появится ситуация, при которой значение текущего кадра станет больше, чем общее кол-во кадров. Необходимо это разрешить. Это можно сделать благодаря нахождению процента от общего количества кадров
		currentFrame = (++currentFrame) % frames.size(); // Благодаря делению с остатком текущий кадр не уйдет в большее значение, чем количество элементов в массиве
		frameClock.restart();
	}

	// определяем когда заканчивается анимация и выставляем значение isActive = false
	// если время анимации подошло к концу, то значит она верно отработала
	if (totalClock.getElapsedTime().asSeconds() > totalDuration)
	{
		isActive = false;
		return true;
	}

	return false;
}

void Animation::Draw(sf::RenderWindow& window)
{
	if (!isActive) return;

	// Создаем объект спрайта и помещаем в него текстуру из массива кадров, обращаясь по индексу текущего кадра
	sf::Sprite frameSprite(*frames[currentFrame]);
	frameSprite.setScale({ 1.0f, 1.0f });
	frameSprite.setPosition({ 500.0f, 260.0f });

	window.draw(frameSprite);
}