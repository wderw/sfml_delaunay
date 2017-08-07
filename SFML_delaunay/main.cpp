#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdlib>
#include <algorithm>

#include "common.h"
#include "Utils.h"
#include "Vertex.h"
#include "IRenderable.h"

int main()
{
	time_t randomSeed = time(NULL);
	srand(randomSeed);

	printf("seed: %d\n", randomSeed);
	//srand(1531);
	//srand(1501925017);

	// zaladuj fonty
	sf::Font font;
	if (!font.loadFromFile("cour.ttf"))
	{
		MessageBoxA(NULL, "Cannot load font file!", "Error", MB_ICONERROR | MB_OK);
	}

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1440, 900), "SFML Window One", sf::Style::Default, settings);

	// vsync
	window.setVerticalSyncEnabled(true);

	sf::Vector2u windowSize = window.getSize();

	/* shapes text */
	sf::Text mousePosText;
	mousePosText.setFont(font);
	mousePosText.setString("Mouse world position.");
	mousePosText.setCharacterSize(18);
	mousePosText.setFillColor(sf::Color::White);
	mousePosText.setStyle(sf::Text::Bold | sf::Text::Regular);
	mousePosText.setPosition(static_cast<double>(mousePosText.getCharacterSize()), static_cast<double>(windowSize.y) - mousePosText.getCharacterSize() * 2);

	/* automatyczne generowanie punktow */
	for (int i = 0; i < VERTEX_COUNT; ++i)
	{
		double quake = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		Vertex* newVertex = new Vertex(rand() % ((int)window.getSize().x - 120) + 60 + quake, rand() % ((int)window.getSize().y - 120) + 60 + quake);
	}

	// zmierz czas wykonania
	sf::Clock clock;
	clock.restart();

	Utils::dt_dewall(Vertex::vertices,(double) 400);

	sf::Time time = clock.restart();
	double timeMilliseconds = time.asMilliseconds();
	//Utils::msgbox(std::to_string(timeMilliseconds));

	printf("elapsed time: %lf", timeMilliseconds);
	//utils::msgbox(std::to_string(e.size()));
	//Utils::dt_bruteforce(Vertex::vertices);

	double moveValue = 1.0;
	// petla komunikatow i obsluga zdarzen
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::KeyPressed:
				{

					if (event.key.code == sf::Keyboard::F)
					{
						moveValue *= 0.5;
					}

					if (event.key.code == sf::Keyboard::G)
					{
						moveValue *= 2.0;
					}
					if (event.key.code == sf::Keyboard::F1)
					{
						sf::View view = window.getView();
						view.zoom(0.5f);
						moveValue *= 0.5;
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::F2)
					{
						sf::View view = window.getView();
						view.zoom(2.0);
						moveValue *= 2.0;
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::W)
					{
						sf::View view = window.getView();
						view.move(0, -moveValue);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::S)
					{
						sf::View view = window.getView();
						view.move(0, moveValue);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::A)
					{
						sf::View view = window.getView();
						view.move(-moveValue, 0);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::D)
					{
						sf::View view = window.getView();
						view.move(moveValue,0);
						window.setView(view);
					}
					else if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						return 0;
					}
					break;
				}
				default:
				{
					break;
				}
			}

		}
		// rysowanie
		window.clear(sf::Color(CUSTOM_BLACK));

		for (int i = 0; i < IRenderable::renderables.size(); ++i)
		{
			IRenderable* pRenderable = IRenderable::renderables.at(i);
			if (pRenderable && pRenderable->isVisible())
			{
				pRenderable->render(&window);
			}
		}

		// wyswietl informacje o aktualnie podswiedlonych wierzcholkach
		size_t n = std::count(mousePosText.getString().begin(), mousePosText.getString().end(), '\n');
		mousePosText.setPosition(mousePosText.getPosition().x, window.getSize().y - mousePosText.getCharacterSize()*n - mousePosText.getCharacterSize() * 2);

		window.draw(mousePosText);
		window.display();
	}
	return 0;
}