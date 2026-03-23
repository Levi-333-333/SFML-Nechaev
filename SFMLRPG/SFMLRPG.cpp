#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    // RenderWindow - это класс, ответственный за создание графического окна. Наследуется от более низкоуровневого кода, который взаимодействует с OpenGL.
    // VideoMode - класс, определяющий свойства окна. Напирмер: его размер.
    RenderWindow mainWindow(VideoMode({ 800, 600 }), "Test");

    // Создание объекта круга
    CircleShape basicCircle(90.f);
    basicCircle.setFillColor(Color::White);
    basicCircle.setOutlineThickness(5.f);
    basicCircle.setOutlineColor(Color::Blue);

    // Установка центра фигуры
    basicCircle.setOrigin({ 20.f, 40.f });
    float basicCircleSpeed = 5.f;
    // Функция setPosition для установки положения объекта в окне
    Vector2f circlePosition = { 310.f, 210.f };
    basicCircle.setPosition(circlePosition);

    while (mainWindow.isOpen())
    {
        // Необходимо обрабатывать события каждый кадр. Если быть точнее, то необходимо обрабатывать события, поступившие с момента прошлого кадра.
        // Происходит создание объекта события, в который помещается обрабатываемое событие окна каждый кадр
        while (const std::optional event = mainWindow.pollEvent())
        {
            // Проверка закрытия окна
            // Обращаясь к событию, которое возникло у окна, через указатель возможно вызвать функцию is, которая в качестве аргумента принимает шаблонное действие события
            if (event->is<Event::Closed>())
            {
                mainWindow.close();
            }
        }

        // Логика вращения круга
        basicCircle.move({ basicCircleSpeed, 0.f });
        basicCircle.move({ -basicCircleSpeed, 0.f });
        basicCircle.rotate(degrees(0.5f));
        basicCircle.setOrigin({ 20.0f, 40.0f });

        // очистка окна, что бы круги не наслаивались друг на друга
        mainWindow.clear(Color(0, 0, 0, 255));
        // Окно является контейнером для того, что необходимо отрисовать на экране. Воспроизвести содержимое можно с помощью функции dispaly()
        mainWindow.draw(basicCircle);
        // Показываем окно
        mainWindow.display();
    }

    return 0;
}