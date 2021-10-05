#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "CollisionQuerier.h"

class CollisionManager;

class Collider {
	friend class CollisionManager;
private:
	sf::RectangleShape* rect;
	std::string id;
	bool active;
public:
	Collider(std::string id, float x, float y, float w, float h) {
		active = true;
		this->id = id;
		rect = new sf::RectangleShape();
		rect->setFillColor(sf::Color::White);
		rect->setOutlineColor(sf::Color::Black);
		rect->setOutlineThickness(4);
		rect->setSize({ w, h });
		rect->setPosition({ x, y });
	}

	Collider(std::string id, float x, float y, float w, float h, std::string filename) : Collider{ id,x,y,w,h } {
		rect->setOutlineThickness(0);
		sf::Texture* tex = new sf::Texture();
		tex->loadFromFile(filename);
		rect->setTexture(tex);
	}


	void draw() {
		if (!active)
			return;
		Globals::window->draw(*rect);
	}
};

class CollisionManager {
private:
	std::vector<Collider*> colliders;
public:
	CollisionManager() {
		//We build the map in here
		colliders.push_back(new Collider("dogcamp", 0, 200, 400, 400, "Assets/dogcamp.png"));
		colliders.push_back(new Collider("tutorialdog", 1000, 400, 200, 200, "Assets/dog.png"));
		colliders.push_back(new Collider("box", 1700, 400, 200, 200, "Assets/crate2.png"));
		colliders.push_back(new Collider("barbedwire", 2200, 400, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("wiredog", 4000, 400, 200, 200, "Assets/dog.png"));
		colliders.push_back(new Collider("barbedwire", 4400, 400, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("barbedwire", 4600, 400, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("barbedwire", 4800, 400, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("barbedwire", 6400, 0, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("prisondog", 5400, 400, 200, 200, "Assets/dog.png"));
		colliders.push_back(new Collider("lazycat", 11550, 500, 200, 200, "Assets/cat.png"));
		colliders.push_back(new Collider("box", 12200, 500, 200, 200, "Assets/crate1.png"));
		colliders.push_back(new Collider("camp", 8900, 200, 400, 400, "Assets/camp.png"));
		colliders.push_back(new Collider("box", 9300, 400, 50, 200, "Assets/crate1.png"));
		colliders.push_back(new Collider("camp", 15200, -100, 400, 400, "Assets/camp.png"));
		colliders.push_back(new Collider("box", 15600, 100, 200, 200, "Assets/crate1.png"));
		colliders.push_back(new Collider("snax1", 9200, -800, 200, 200, "Assets/treat.png"));
		colliders.push_back(new Collider("snax2", 16600, -1000, 200, 200, "Assets/treat.png"));
		colliders.push_back(new Collider("box", 13400, 0, 300, 300, "Assets/crate1.png"));
		colliders.push_back(new Collider("barbedwire", 13700, 100, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("barbedwire", 13900, 100, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("barbedwire", 19000, 100, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("catnipcat", 17600, 100, 200, 200, "Assets/cat.png"));
		colliders.push_back(new Collider("box", 20200, 500, 300, 300, "Assets/crate2.png"));
		colliders.push_back(new Collider("ammodog", 20500, 600, 200, 200, "Assets/dog.png"));
		colliders.push_back(new Collider("barbedwire", 21500, 600, 200, 200, "Assets/barbedwire.png"));
		colliders.push_back(new Collider("tutorialcat", 22400, 600, 200, 200, "Assets/cat.png"));
		colliders.push_back(new Collider("catcamp", 23300, 400, 400, 400, "Assets/catcamp.png"));
		colliders.push_back(new Collider("nuisancecrate", 21900, 500, 300, 300, "Assets/crate1.png"));
		colliders.push_back(new Collider("box", 21700, 300, 200, 500));
		colliders.push_back(new Collider("ground", -1000, 600, 11000, 800));
		colliders.push_back(new Collider("killbox", -1000, 2000, 25000, 10));
		colliders.push_back(new Collider("platform", 18200, 300, 200, 25));
		colliders.push_back(new Collider("platform", 18250, 500, 200, 25));
		colliders.push_back(new Collider("platform", 18800, 300, 200, 25));
		colliders.push_back(new Collider("platform", 16200, 0, 200, 25));
		colliders.push_back(new Collider("platform", 17000, 0, 200, 25));
		colliders.push_back(new Collider("platform", 16600, -300, 200, 25));
		colliders.push_back(new Collider("platform", 4600, 300, 200, 25));
		colliders.push_back(new Collider("platform", 5400, 200, 200, 25));
		colliders.push_back(new Collider("platform", 5425, 300, 200, 25));
		colliders.push_back(new Collider("platform", 6000, 200, 200, 25));
		colliders.push_back(new Collider("platform", 7300, 300, 200, 25));
		colliders.push_back(new Collider("platform", 7900, 300, 200, 25));
		colliders.push_back(new Collider("platform", 8300, 0, 200, 25));
		colliders.push_back(new Collider("platform", 8900, 0, 200, 25));
		colliders.push_back(new Collider("platform", 9500, -300, 200, 25));
		colliders.push_back(new Collider("platform", 10000, 775, 200, 25));
		colliders.push_back(new Collider("platform", 10650, 775, 200, 25));
		colliders.push_back(new Collider("platform", 11300, 775, 200, 25));
		colliders.push_back(new Collider("platform", 14500, 0, 200, 25));
		colliders.push_back(new Collider("platform", 14000, -300, 200, 25));

		colliders.push_back(new Collider("ground2", 12500, 300, 5700, 1600));
		colliders.push_back(new Collider("ground3", 18200, 800, 5500, 600));
		colliders.push_back(new Collider("box", 19000, 300, 200, 200));
		colliders.push_back(new Collider("box", 11500, 700, 800, 600));
		colliders.push_back(new Collider("box", 6200, 200, 400, 150));
		colliders.push_back(new Collider("box", 5000, 200, 400, 400));
		colliders.push_back(new Collider("box", 8500, 0, 400, 600));
		colliders.push_back(new Collider("box", 8500, -1000, 400, 700));
		colliders.push_back(new Collider("box", 9700, -1000, 400, 1300));
		colliders.push_back(new Collider("wall", 2000, 200, 200, 400));
		
	}

	void remove(std::string id) {
		for (Collider* c : colliders) {
			if (c->id == id) {
				c->active = false;
				return;
			}
		}
	}

	void restore() {
		for (Collider* c : colliders) {
			c->active = true;
		}
	}

	std::vector<std::string> collides(sf::FloatRect bounds) {
		std::vector<std::string> result;
		for (Collider* c : colliders) {
			if (c->active && bounds.intersects(c->rect->getGlobalBounds())) {
				result.push_back(c->id);
			}
		}
		return result;
	}
	
	bool doesCollide(sf::FloatRect bounds) {
		return collides(bounds).size() != 0;
	}


	void draw() {
		for (Collider* c : colliders) {
			c->draw();
		}
	}

	void move(float x, float y) {
		for (Collider* c : colliders) {
			c->rect->move({ x, y });
		}
	}
};