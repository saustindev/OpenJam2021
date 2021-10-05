#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "Collision.h"
#include "BulletManager.h"
#include "PlayerUI.h"

class Enemy {
private:
	friend class EnemyManager;
	PlayerUI* ui;
	bool dog;
	sf::RectangleShape* rect;
	CollisionManager* col;
	BulletManager* bul;
	bool aligned;
	static constexpr float w = 200;
	static constexpr float h = 200;
	static constexpr float maxHP = 40;
	float curHP;
	sf::Sound* die;
	sf::SoundBuffer* dieBuf;
	sf::Sound* pain;
	sf::SoundBuffer* painBuf;
	sf::Sound* shot;
	sf::SoundBuffer* shotBuf;
	bool dead;
	sf::Clock clock;
	static constexpr float shootTime = .75;
public:
	Enemy(bool dog, float x, float y, CollisionManager *col, BulletManager *bul, PlayerUI* ui) {
		this->ui = ui;
		this->bul = bul;
		dead = false;
		die = new sf::Sound();
		dieBuf = new sf::SoundBuffer();
		pain = new sf::Sound();
		painBuf = new sf::SoundBuffer();
		shot = new sf::Sound();
		shotBuf = new sf::SoundBuffer();
		shotBuf->loadFromFile("Assets/BG/bang (6).ogg");
		shot->setBuffer(*shotBuf);
		curHP = maxHP;
		this->dog = dog;
		rect = new sf::RectangleShape();
		aligned = false;
		this->col = col;
		sf::Texture* tex = new sf::Texture();
		if (dog) {
			dieBuf->loadFromFile("Assets/cat.ogg");
			tex->loadFromFile("Assets/guncat.png");
		}
		else {
			dieBuf->loadFromFile("Assets/dog.ogg");
			tex->loadFromFile("Assets/gundog.png");
		}
		die->setBuffer(*dieBuf);
		painBuf->loadFromFile("Assets/hit.ogg");
		pain->setBuffer(*painBuf);
		rect->setTexture(tex);
		rect->setOrigin(w / 2, 0);
		rect->setPosition(x, y);
		rect->setSize({ w, h });
	}

	void stop() {
		die->stop();
		pain->stop();
		shot->stop();
		delete(shot);
		delete(shotBuf);
		delete(die);
		delete(pain);
		delete(dieBuf);
		delete(painBuf);
	}

	void update() {
		if (dead)
			return;

		//Flip towards player
		if (rect->getPosition().x > Globals::windowX / 2) {
			rect->setScale(-1, 1);
		}
		else {
			rect->setScale(1, 1);
		}

		//Fall to nearest ground
		if (!aligned) {
			rect->move(0, 1);
			if (col->doesCollide(rect->getGlobalBounds())) {
				rect->move(0, -1);
				aligned = true;
			}
		}

		//Shoot at player if they are in sight
		if (clock.getElapsedTime().asSeconds() > shootTime && rect->getPosition().x > 0 && rect->getPosition().x < Globals::windowX && rect->getPosition().y > 0 && rect->getPosition().y < Globals::windowY) {
			bul->createBullet(false, rect->getPosition().x + (rect->getScale().x * rect->getSize().x / 2), rect->getPosition().y + (dog ? 50 : 100), -90.f + (float)rect->getScale().x * 90.f);
			clock.restart();
			shot->play();
		}
	}

	void move(float x, float y) {
		rect->move(x, y);
	}

	void damage() {
		curHP -= 10;
		if (curHP <= 0) {
			dead = true;
			die->play();
			ui->addPoints(100);
		}
		else {
			pain->play();
		}
	}

	void draw() {
		if(!dead)
			Globals::window->draw(*rect);
	}

	void revive() {
		dead = false;
		curHP = maxHP;
	}
};

class EnemyManager {
private:
	bool dog;
	std::vector<Enemy*> enemies;
	CollisionManager* col;
	BulletManager* bul;
	PlayerUI* ui;
public:
	EnemyManager(bool dog, CollisionManager *col, BulletManager *bul, PlayerUI* ui) {
		this->dog = dog;
		this->col = col;
		this->bul = bul;
		this->ui = ui;

		//Place enemies in the map
		enemies.push_back(new Enemy(dog, 3500, 300, col, bul, ui));
		enemies.push_back(new Enemy(dog, 6100, 0, col, bul, ui));
		enemies.push_back(new Enemy(dog, 7000, 300, col, bul, ui));
		enemies.push_back(new Enemy(dog, 7900, 0, col, bul, ui));
		enemies.push_back(new Enemy(dog, 7900, 350, col, bul, ui));
		enemies.push_back(new Enemy(dog, 8900, -250, col, bul, ui));
		enemies.push_back(new Enemy(dog, 9500, -600, col, bul, ui));
		enemies.push_back(new Enemy(dog, 13000, 0, col, bul, ui));
		enemies.push_back(new Enemy(dog, 13300, 0, col, bul, ui));
		enemies.push_back(new Enemy(dog, 14600, -200, col, bul, ui));
		enemies.push_back(new Enemy(dog, 14900, 0, col, bul, ui));
		enemies.push_back(new Enemy(dog, 15300, -400, col, bul, ui));
		enemies.push_back(new Enemy(dog, 16700, -600, col, bul, ui));
		enemies.push_back(new Enemy(dog, 16300, -300, col, bul, ui));
		enemies.push_back(new Enemy(dog, 17100, -300, col, bul, ui));
		enemies.push_back(new Enemy(dog, 16700, 0, col, bul, ui));
		enemies.push_back(new Enemy(dog, 18350, 550, col, bul, ui));
		enemies.push_back(new Enemy(dog, 18750, 400, col, bul, ui));
		enemies.push_back(new Enemy(dog, 19350, 400, col, bul, ui));
		enemies.push_back(new Enemy(dog, 19600, 400, col, bul, ui));
		enemies.push_back(new Enemy(dog, 21000, 500, col, bul, ui));
	}

	void update() {
		for (Enemy* e : enemies) {
			if (!e->dead &&bul->collides(false, e->rect->getGlobalBounds())) {
				e->damage();
			}
			e->update();
		}
	}

	void draw() {
		for (Enemy* e : enemies) {
			e->draw();
		}
	}

	void stop() {
		for (Enemy* e : enemies) {
			e->stop();
		}
	}

	void revive() {
		for (Enemy* e : enemies) {
			e->revive();
		}
	}

	void move(float x, float y) {
		for (Enemy* e : enemies) {
			e->move(x, y);
		}
	}

};