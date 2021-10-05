#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Globals.h"

class BulletManager {
private:
	std::vector<Bullet*> bullets;
public:
	BulletManager() {

	}

	void draw() {
		for (Bullet* b : bullets) {
			b->draw();
		}
	}

	void update() {
		for (Bullet* b : bullets) {
			b->update();
		}
	}

	bool collides(bool friendly, sf::FloatRect bounds) {
		for (Bullet* b : bullets) {
			if (b->rect->getPosition().x < 0 || b->rect->getPosition().x > Globals::windowX || b->rect->getPosition().y < 0 || b->rect->getPosition().y > Globals::windowY) {
				b->active = false;
			}
			if (b->active && friendly != b->friendly && bounds.intersects(b->rect->getGlobalBounds())) {
				b->active = false;
				return true;
			}
		}
		return false;
	}
	void move(float x, float y) {
		for (Bullet* b : bullets) {
			b->move(x, y);
		}
	}

	void createBullet(bool dog, float x, float y, double angle) {
		bullets.push_back(new Bullet(dog, x, y, angle));
	}
};