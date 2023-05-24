#include "Menu.h"
#include <iostream>



Menu::Menu(float width, float height)
{

	if (!font.loadFromFile("Fonts/arial.ttf"))
	{
		std::cout << "Couldn't load font ";
	}


	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color(28,108,208));
	menu[0].setString("Play");
	menu[0].setPosition(sf::Vector2f(600, 262));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Scores");
	menu[1].setPosition(sf::Vector2f(585, 388));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Exit");
	menu[2].setPosition(sf::Vector2f(600, 520));

	selectedItemIndex = 0;
}


Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color(28,100,208));
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color(28, 100, 208));
	}
}
