#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include "GameManager.h"

class Bullet {
private:
	friend class BulletManager;
	bool active;
	bool friendly;
	double xVel;
	double yVel;
	sf::RectangleShape* rect;
	static constexpr float sizeX = 30;
	static constexpr float sizeY = 15;
	static constexpr float vel = 1000;
	sf::Clock clock;
public:
	Bullet(bool friendly, float x, float y, double angle) {
		active = true;
		this->friendly = friendly;
		rect = new sf::RectangleShape();
		rect->setSize({ sizeX, sizeY });
		rect->setOrigin({ sizeX / 2, sizeY / 2 });
		rect->setPosition({ x, y });
		rect->setRotation((float)angle);
		rect->setFillColor(sf::Color::Yellow);
		rect->setOutlineColor(sf::Color::Black);
		rect->setOutlineThickness(1);
		xVel = vel * cos(angle*M_PI/180.f);
		yVel = vel * sin(angle * M_PI / 180.f);
	}

	void update() {
		if (!active)
			return;
		float dt = clock.restart().asSeconds();
		rect->move({ (float)xVel * dt, (float)yVel * dt});
	}

	void draw() {
		if (!active)
			return;
		Globals::window->draw(*rect);
	}

	void move(float x, float y) {
		rect->move(x, y);
	}
};