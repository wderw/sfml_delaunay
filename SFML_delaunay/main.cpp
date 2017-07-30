#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdlib>
#include <algorithm>

#include "delaunay_utils.h"

int main()
{
	srand(time(NULL));
	//srand(1530);

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
	mousePosText.setPosition(static_cast<float>(mousePosText.getCharacterSize()), static_cast<float>(windowSize.y) - mousePosText.getCharacterSize() * 2);

	/* automatyczne generowanie punktow */
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		float quake = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		vertex* newVertex = new vertex(rand() % ((int)window.getSize().x - 120) + 60 + quake, rand() % ((int)window.getSize().y - 120) + 60 + quake);
	}


	sf::Clock clock;
	clock.restart();

	utils::dt_dewall(vertex::vertices);

	sf::Time t = clock.restart();
	float time_f = t.asMilliseconds();
	utils::msgbox(std::to_string(time_f));

	//utils::msgbox(std::to_string(e.size()));
	//utils::dt_bruteforce(vertex::vertices);


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
				case sf::Event::MouseMoved:
				{
					/* wyswietl pozycje kursora */
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					std::string s_MousePos = utils::vector_description(&mousePos, "Mouse");
					mousePosText.setString(s_MousePos);

					// podswietl wierzcholki bliskie kursora 
					// uwaga: ultra nieefektywne 				
					for (int i = 0; i < IRenderable::renderables.size(); i++)
					{
						IRenderable* pRenderable = IRenderable::renderables[i];
						vertex* highlightCandidate = dynamic_cast<vertex*>(pRenderable);

						if (highlightCandidate != nullptr)
						{
							sf::Vector2f difference = mousePos - highlightCandidate->position;
							float distance = utils::vector_magnitude(&difference);
							if (distance < VERTEX_SNAP_DISTANCE)
							{
								// highlight
								highlightCandidate->shape.setRadius(4.5f);
								highlightCandidate->shape.setOutlineThickness(4.5f);
								highlightCandidate->outlineColor = sf::Color(CUSTOM_GREEN);

								std::string sVertexInfo = utils::vector_description(&highlightCandidate->position, "\nCurrent vertex");
								mousePosText.setString(mousePosText.getString() + sVertexInfo);
							}
							else
							{
								// unhighlight
								highlightCandidate->shape.setRadius(3.0f);
								highlightCandidate->shape.setOutlineThickness(2.0f);
								highlightCandidate->outlineColor = sf::Color(CUSTOM_RED);
							}
						}
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

		for (int i = 0; i < IRenderable::renderables.size(); i++)
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