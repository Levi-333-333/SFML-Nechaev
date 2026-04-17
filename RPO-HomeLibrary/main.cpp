#include <SFML/Graphics.hpp>
#include <vector>
#include "Animation.h"
#include "Book.h"
#include "Button.h"
#include "InputField.h"
#include "colors.h"

#define BookTitleField inputFields[0]
#define AuthorField inputFields[1]
#define YearField inputFields[2]

int main()
{
	// ---------------Загрузка ресурсов---------------
	sf::Font font("Comic Sans MS.ttf");
	sf::Texture bookTexture("book1.png");
	Animation loadingAnimation;
	// В моем проекте кадры анимации загрузки находятся в папке animations в корне проекта
	loadingAnimation.LoadFrames({
		"animations\\1.png",
		"animations\\2.png",
		"animations\\3.png",
		"animations\\4.png",
		"animations\\5.png",
		"animations\\6.png",
		});

	// ---------------Инициализация основного окна---------------
	const unsigned int WINDOW_WIDTH = 800;
	const unsigned int WINDOW_HEIGHT = 600;
	sf::RenderWindow mainWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), L"Домашняя библиотека", sf::Style::Close);
	// Добавление иконки
	sf::Image icon("icon.png");
	mainWindow.setIcon(icon);

	// ---------------Создание объектов---------------
	sf::RectangleShape leftPanel({ 350.0f, 565.0f });
	leftPanel.setPosition({ 15.0f, 15.0f });
	leftPanel.setFillColor(sf::Color(ROSY_GRANITE_COLOR));
	leftPanel.setOutlineColor(sf::Color(CHARCOAL_BROWN_COLOR));
	leftPanel.setOutlineThickness(5.0f);

	sf::RectangleShape rightPanel({ 350.0f, 565.0f });
	rightPanel.setPosition({ 430.0f, 15.0f });
	rightPanel.setFillColor(sf::Color(ROSY_GRANITE_COLOR));
	rightPanel.setOutlineColor(sf::Color(CHARCOAL_BROWN_COLOR));
	rightPanel.setOutlineThickness(5.0f);

	sf::Text statusText(font, "", 18);
	statusText.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
	statusText.setPosition({ 90.0f, 540.0f });
	// Объекты для статуса добавления книги
	sf::String statusMessage;
	bool showStatus = false;
	sf::Clock statusClock;

	auto SetStatus = [&](sf::String& message)
		{
			statusMessage = message;
			showStatus = true;
			statusClock.restart();
		};

	std::vector<Book> booksArray;
	booksArray.push_back(Book{L"Искусство языка Си", L"Сунь Си", L"2027.04.03"});
	booksArray.push_back(Book{L"Отруби", L"Валентин Касевьев Омарович", L"-2000.-0.1.-5"});

	// Размер по x - количество букв * 10
													// Размер по y - размер символа * 2
	InputField bookTitleField(font, L"Название книги", { 20.0f, 100.0f }, { 320.0f, 36.f });
	InputField authorField(font, L"Автор книги", { 20.0f, 200.0f }, { 320.0f, 36.f });
	InputField yearField(font, L"Год издания", { 20.0f, 300.0f }, { 320.0f, 36.f });
	// Лист нужен для функции PutActive
	std::vector<InputField> inputFields = { bookTitleField, authorField, yearField };

	auto PutActive = [&](InputField necessaryInputField)
		{
			for (int i = 0; i < inputFields.size(); i++)
			{
				if (inputFields[i].GetCaptionText().getString() == necessaryInputField.GetCaptionText().getString()) inputFields[i].SetActive(true);
				else inputFields[i].SetActive(false);
			}
		};

	auto AddBook = [&]()
		{
			if (BookTitleField.GetValue().isEmpty() || AuthorField.GetValue().isEmpty() || YearField.GetValue().isEmpty())
			{
				sf::String message(L"Не все поля заполнены!");
				SetStatus(message);
				return;
			}

			// Только тогда, когда все время, отведенное на анимацию, завершится, мы добавляем книгу в массив для отрисовки на экране
			booksArray.push_back(Book{ BookTitleField.GetValue(), AuthorField.GetValue(), YearField.GetValue() });

			BookTitleField.ClearInputValue();
			YearField.ClearInputValue();
			AuthorField.ClearInputValue();

			loadingAnimation.StartAnimation();
			sf::String message(L"Добавление книги...");
			SetStatus(message);
		};				

	auto OpenBook = [&]()
	{
		sf::RenderWindow bookWindow(sf::VideoMode({ 400, 200 }), L"Карточка книги", sf::Style::Close);
		while (mainWindow.isOpen())
		{
			while (const std::optional event = bookWindow.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					bookWindow.close();
				}
			}
		}		
		bookWindow.clear(sf::Color(SUNFLOWER_GOLD_COLOR));
		bookWindow.display();
	};

	Button addBookButton(font, L"Добавить книгу", { 90.0f, 500.0f }, { 200.0f, 26.0f });
	Button openBookButton = { font, L"Открыть", {680.0f, 60.0f}, {90, 30} };

	// ---------------Обработка событий---------------
	while (mainWindow.isOpen())
	{
		// Обработка события по взаимодействию с главным окном
		while (const std::optional event = mainWindow.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				mainWindow.close();
			}
			// ИЗМЕНЕНО: пользователь может взаимодействовать с программой только если анимация загрузки книги в библиотеку не активна!
			else if (!loadingAnimation.GetIsActive())
			{
				// дополнительная обработка события нахождения мыши в пределах окна и нажатия. getIF - вспомогательная функция, определяющая наличие события с дополнительным уссловием
				if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
				{
					if (mouseEvent->button == sf::Mouse::Button::Left)
					{
						// Создание объекта вектора, показывающего координаты нажатия курсора мыши
						sf::Vector2f mousePoint = { float(mouseEvent->position.x), float(mouseEvent->position.y) };

						// Если клик мыши находится в каком-либо из графических объектов на экране, активируем эти объекты
						if (BookTitleField.Contains(mousePoint)) PutActive(BookTitleField);
						else if (AuthorField.Contains(mousePoint)) PutActive(AuthorField);
						else if (YearField.Contains(mousePoint)) PutActive(YearField);
						else
						{
							BookTitleField.SetActive(false);
							AuthorField.SetActive(false);
							YearField.SetActive(false);
						}
						// Обработка нажатия на кнопку
						if (addBookButton.Contains(mousePoint))
						{
							AddBook();
						}
						else if (openBookButton.Contains(mousePoint))
						{
							OpenBook();
						}
					}
				}
				// Обработка события печатания
				else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
				{
					BookTitleField.HandleTextEntered(textEntered->unicode);
					AuthorField.HandleTextEntered(textEntered->unicode);
					YearField.HandleTextEntered(textEntered->unicode);
				}
			}
		}

		if (loadingAnimation.UpdateAnimation())
		{
			sf::String message(L"Книга успешно добавлена!");
			SetStatus(message);
		}

		// Очистка текста статуса
		if (showStatus && statusClock.getElapsedTime().asSeconds() > 2.0f) showStatus = false;
		
		// ---------------Очистка элементов---------------
		mainWindow.clear(sf::Color(227, 178, 60));

		// ---------------Отрисовка элементов---------------
		mainWindow.draw(leftPanel);
		mainWindow.draw(rightPanel);
		AuthorField.Draw(mainWindow);
		YearField.Draw(mainWindow);
		BookTitleField.Draw(mainWindow);
		addBookButton.Draw(mainWindow);

		// Если статус добавления книги есть, то отрисовываем его
		if (showStatus)
		{
			statusText.setString(statusMessage);
			mainWindow.draw(statusText);
		}

		// Создаем объект текста для вывода количества книг
		sf::Text bookCountText(font, "", 18);
		bookCountText.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
		bookCountText.setString(sf::String(L"Всего книг: ") + sf::String(std::to_string(booksArray.size())));
		bookCountText.setPosition({ 440.0f, 30.0f });
		mainWindow.draw(bookCountText);

		// Если массив книг пуст, то создаем текст один, в другом случае - другой
		if (booksArray.empty())
		{
			sf::Text emptyText(font, L"Библиотека пуста", 18);
			emptyText.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
			emptyText.setPosition({ 440.0f, 60.0f });
			mainWindow.draw(emptyText);
		}
		else
		{
			// Перечисляем все библиотеки из массива и создаем под каждую из них свой объект текста
			float bookFieldY = 60.0f;
			for (int i = 0; i < booksArray.size(); ++i)
			{
				const Book& book = booksArray[i];
				// Формат вывода книги: 1. Название / автор. - год
				//sf::String bookInfo = sf::String(std::to_string(i + 1)) + ". " + book.title + " / " + book.author + ". \n- " + book.year;👿
				sf::String bookInfo = sf::String(std::to_string(i + 1)) + ". " + book.title;

				// Отрисовка спрайта в программе.
				// По аналогии с отрисовкой текста в графическом приложении, где есть два вида классов Text и String, у отрисовки пользовательской (своей) графики есть класс Texture, отвечающий за содержимое изображения, и класс Sprite, отвечающий за рендер или отрисовку в графическом окне.
				sf::Sprite bookSprite(bookTexture);
				bookSprite.setScale({ 0.01f, 0.01f });
				bookSprite.setPosition({ 430.0f, bookFieldY });
				mainWindow.draw(bookSprite);

				sf::Text bookLine(font, bookInfo, 18);
				bookLine.setFillColor(sf::Color(CHARCOAL_BROWN_COLOR));
				bookLine.setPosition({ 480.0f,  bookFieldY });

				openBookButton.buttonShape.setPosition({ 680.0f, bookFieldY });
				openBookButton.Draw(mainWindow);
				mainWindow.draw(bookLine);

				bookFieldY += 50.0f;
			}
		}
		loadingAnimation.Draw(mainWindow);
		mainWindow.display();
	}	

	return 0;
}