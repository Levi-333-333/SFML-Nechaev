#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

using namespace sf;

// Структура пули, имеющая фигуру прямоугольника и флаг, говорящий о существовании объекта
struct Bullet
{
    CircleShape bulletShape;
    bool isDestroyed = false;
};
// Структура врага, имеющая фигуру прямоугольника и флаг, говорящий о существовании объекта
struct Enemy
{
    RectangleShape enemyShape;
    bool isDestroyed = false;
};

int main()
{    
    // Инициализация окна
    unsigned int width = 1024;
    unsigned int height = 768;
    RenderWindow mainWindow(VideoMode({ width, height }), "RPO Invaders");

    // Инициализация игрока
    RectangleShape playerShape;
    playerShape.setSize({ 60.0f, 20.0f });
    playerShape.setFillColor(Color::Yellow);
    playerShape.setOutlineThickness(5.0f);
    playerShape.setOutlineColor(Color::Blue);
    playerShape.setOrigin({ 30.0f, 10.0f });
    playerShape.setPosition({ width / 2.0f, height - 10.0f });    

    // Инициализация других объектов
    std::vector<Bullet> bulletsArray;
    std::vector<Enemy> enemiesArray;
    
    // Рандомизация
    // mt19937 - генератор случайных чисел, основанный на алгоритме Мерсенна, который генерирует РАВНОМЕРНО распределенные числа с большим периодом (2^19937 - 1).
    // random_device - класс, генерирующий истинно случайные числа, получающиеся от вычислений железа компьютера.
    std::mt19937 rng(std::random_device{}());
    // uniform_real_distribution - применяется для указания диапазона значений
    std::uniform_real_distribution<float> xDistribution(20.0f, width - 20.0f);

    // Таймеры
    Clock framesClock; // используется для deltaTime
    Clock spawnEnemyClock;
    Clock spawnBulletClock;

    // Скорость объектов
    const float playerSpeed = 200.0f;
    const float enemySpeed = 175.0f;
    const float bulletSpeed = 400.0f;

    // Флаг конца игры
    bool isGameOver = false;

    while (mainWindow.isOpen() && !isGameOver)
    {        
        // Обработка события по взаимодействию с главным окном
        while (const std::optional event = mainWindow.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                mainWindow.close();
            }
        } 

        // Логика игрока
        float playerDirection = 0.0f; // Направление игрока влево вправо
        // delta time - отношение кадра к предыдущему. Умножать на данное значение необходимо затем, чтобы количество кадров при разном количестве кадров в секунду, было одинаковым.
        float deltaTime = framesClock.restart().asSeconds();
        // Если нажата клавиша A или Стрелка влево, то изменяем направление движения фигуры игрока
        if (Keyboard::isKeyPressed(Keyboard::Key::A) || Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            playerDirection -= 1.0f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::D) || Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            playerDirection += 1.0f;
        }

        // ---------------ПЕРЕДВИЖЕНИЕ И ДЕЙСТВИЯ ИГРОКА---------------
        playerShape.move({ playerDirection * playerSpeed * deltaTime, 0.0f });
        // Ограничение передвижение игрока границами экрана
        // Если половина размера фигуры игрока становится меньше значения x игрока, то перемещаем игрока обратно
        float halfPlayerWidth = playerShape.getScale().x / 2;
        float playerCurrentX = playerShape.getPosition().x;
        // Левая часть экрана
        if (playerCurrentX < halfPlayerWidth) playerShape.setPosition({ halfPlayerWidth, playerShape.getPosition().y });
        // Правая часть экрана
        if (playerCurrentX > width - halfPlayerWidth) playerShape.setPosition({ width - halfPlayerWidth, playerShape.getPosition().y });

        // ---------------Инициализация пуль---------------
        // getElapsedTime возвращает пройденное время с последнего момента обновления таймера. Если оно больше 3 милисекунд, то можем стрелять
        if (Keyboard::isKeyPressed(Keyboard::Key::Space) && spawnBulletClock.getElapsedTime().asSeconds() > 0.3f)
        {
            Bullet bullet;
            bullet.bulletShape = CircleShape(5.0f);
            bullet.bulletShape.setFillColor(Color::Yellow);
            bullet.bulletShape.setPosition({ playerShape.getPosition().x, playerShape.getPosition().y - 20.0f });
            bulletsArray.push_back(bullet);
            spawnBulletClock.restart(); // Обновление таймера
        }

        // ---------------Инициализация врагов---------------
        if (spawnEnemyClock.getElapsedTime().asSeconds() > 1.0f)
        {
            Enemy enemy;
            enemy.enemyShape = RectangleShape({ 40.0f, 30.0f });
            enemy.enemyShape.setFillColor(Color::Red);
            enemy.enemyShape.setOutlineColor(Color::White);
            enemy.enemyShape.setOutlineThickness(5.0f);

            // В качестве позиции x используется рандомизированное число xDistribution с соответствующим seed
            enemy.enemyShape.setPosition({ xDistribution(rng), -9.0f });

            enemiesArray.push_back(enemy);
            spawnEnemyClock.restart();
        }

        // ---------------Обновление объектов---------------
        // Обновление пуль
        for (Bullet& bullet : bulletsArray)
        {
            bullet.bulletShape.move({ 0.0f, -bulletSpeed * deltaTime });
            if (bullet.bulletShape.getPosition().y < -20.0f) bullet.isDestroyed = true;
        }

        // Обновление врагов
        for (Enemy& enemy : enemiesArray)
        {
            enemy.enemyShape.move({ 0.0f, enemySpeed * deltaTime });

            // ЕСЛИ ВРАГ СТОЛКНУЛСЯ С ИГРОКОМ, ТО КОНЕЦ ИГРЫ (isGameOver = true)
            // getGlobalBounds позволяет найти точки фигуры в координатном пространстве графического окна. Если затем применить findIntersection и передать возвращаемое значение getGlobalBounds другого shape-объекта, то можно проверить пересекаются ли объекты
            if (enemy.enemyShape.getGlobalBounds().findIntersection(playerShape.getGlobalBounds())) isGameOver = true;
        }

        // ---------------Обработка коллизий пуль и врагов---------------
        for (Bullet& bullet : bulletsArray)
        {
            if (bullet.isDestroyed) continue;
            for (Enemy& enemy : enemiesArray)
            {
                if (enemy.isDestroyed) continue;

                if (bullet.bulletShape.getGlobalBounds().findIntersection(enemy.enemyShape.getGlobalBounds()))
                {
                    bullet.isDestroyed = true;
                    enemy.isDestroyed = true;
                    break;
                }
            }
        }

        // ---------------Очистка объектов---------------
        // Для того, чтобы в массиве перечислить от начала до конца объекты можно использовать функцию remove_if, которая помимо начала и конца принимает условие очистки массива. В качестве условия мы используем лямбда выражение, которое в перечисляемом объекте пули проверяет флаг isDestroyed
        // Очистка пуль
        bulletsArray.erase(std::remove_if(bulletsArray.begin(), bulletsArray.end(), [](Bullet& bullet) {return bullet.isDestroyed; }), bulletsArray.end());

        // Очистка врагов
        enemiesArray.erase(std::remove_if(enemiesArray.begin(), enemiesArray.end(), [](Enemy& enemy) {return enemy.isDestroyed; }), enemiesArray.end());

        // ---------------Отрсовка главного окна---------------
        mainWindow.clear(Color(0, 0, 0, 255)); 

        for (Bullet& bullet : bulletsArray) mainWindow.draw(bullet.bulletShape);
        for (Enemy& enemy: enemiesArray) mainWindow.draw(enemy.enemyShape);

        mainWindow.draw(enemiesArray[0].enemyShape);
        mainWindow.draw(playerShape);
        mainWindow.display();
    }

    return 0;
}