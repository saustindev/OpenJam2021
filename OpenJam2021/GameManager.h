#pragma once
#include <vector>
#include <SFML/Audio.hpp>
#include "Updateable.h"
#include "Globals.h"

namespace Globals {
	bool runGame = true;
}

class GameManager : public Updateable {
private:
	std::vector<Updateable*> updateables;
	std::vector<std::string> ids;
	Updateable* active;
	sf::Music* mus;
	std::string activeID;

public:
	GameManager() {
		active = nullptr;
		mus = new sf::Music();
		mus->openFromFile("Assets/menu.ogg");
		mus->setVolume(25);
		mus->setLoop(true);
		mus->play();
	}

	void add(Updateable* u, std::string id) {
		updateables.push_back(u);
		ids.push_back(id);
		if (ids.size() == 1) {
			active = u;
		}
	}

	void swap(std::string id) {
		for (int i = 0; i < ids.size(); i++) {
			if (ids.at(i) == id) {
				activeID = id;
				active = updateables.at(i);
				return;
			}
		}
	}

	std::string getId() {
		return activeID;
	}

	void stop() {
		Globals::runGame = false;
		mus->stop();
		delete(mus);
		for (Updateable* u : updateables) {
			u->stop();
		}
	}

	void update() {
		if (active == nullptr)
			return;
		active->update();
	}

	void draw() {
		if (active == nullptr)
			return;
		active->draw();
	}

	void click() {
		if (active == nullptr)
			return;
		active->click();
	}

};