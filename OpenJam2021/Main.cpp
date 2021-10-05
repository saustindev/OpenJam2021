#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "Menu.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "Game.h"


int main() {
	sf::RenderWindow win(sf::VideoMode(Globals::windowX, Globals::windowY), "Open Jam 2021", sf::Style::Close | sf::Style::Resize);
    sf::View view = win.getDefaultView();
    Globals::window = &win;
    Globals::font = new sf::Font();
    Globals::font->loadFromFile("Assets/typed.ttf");

    Globals::game = new GameManager();
    Globals::net = new NetworkManager();
    Globals::game->add(new MainMenu(), "main");
    Globals::game->add(new ExpositionMenu(), "expo");
    Globals::game->add(new ChooseMenu(), "choose");
    Globals::game->add(new Game(true), "doggame");
    Globals::game->add(new Game(false), "catgame");
    Globals::game->add(new AboutMenu(), "about");
    Globals::game->add(new ScoreMenu(), "score");

	sf::RectangleShape* rect = new sf::RectangleShape();

    while (win.isOpen() && Globals::runGame)
    {
        //WINDOW EVENTS
        sf::Event event;
        while (win.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                Globals::game->stop();
            }
            else if (event.type == sf::Event::Resized) {    
                Globals::conditionalResize(Globals::game->getId());
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Globals::game->click();
                }
            }

        }

        //LOGIC
        sf::Vector2i mouseCoords = sf::Mouse::getPosition(win);
        Globals::mouseX = mouseCoords.x;
        Globals::mouseY = mouseCoords.y;
        Globals::game->update();
        Globals::conditionalResize(Globals::game->getId());

        //GRAPHICS
        win.clear(sf::Color::White);
        //Render here
        Globals::game->draw();
        //Then display
        win.display();
    }
    win.close();
	return 0;
}