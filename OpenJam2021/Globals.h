#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "GameManager.h"
#include "NetworkManager.h"

namespace Globals {
	constexpr int defX = 1200;
	constexpr int defY = 800;
	int windowX = defX;
	int windowY = defY;
	int mouseX = 0;
	int mouseY = 0;
	sf::RenderWindow *window = nullptr;
	sf::Font* font = nullptr;
	GameManager* game = nullptr;
	NetworkManager* net = nullptr;
	extern bool runGame;

	void conditionalResize(std::string id) {
		if (id == "doggame" || id == "catgame") {
			windowX = defX;
			windowY = defY;
		}
		else {
			windowX = window->getSize().x;
			windowY = window->getSize().y;
		}
	}
}