#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Updateable.h"
#include "Player.h"
#include "Bullet.h"
#include "Collision.h"


class Game : public Updateable {
private:
	Player* player;
	CollisionManager* col;
	
	sf::Sound *shot;
	sf::SoundBuffer *shotb;
	
	
public:
	Game(bool dog) {
		col = new CollisionManager();
		player = new Player(dog, col);
		shot = new sf::Sound();
		shotb = new sf::SoundBuffer();
		shotb->loadFromFile("Assets/bang.ogg");
		shot->setBuffer(*shotb);
		
		
	}
	void draw() {
		player->draw();
		
	}

	void update() {
		player->update();
	}

	void click() {
		if (!player->click()) {
			sf::Vector2f pos = player->getPosition();
			player->makeBullet();
			shot->play();
			//std::cout << "bang: " << std::to_string(pos.x) << "," << std::to_string(pos.y) << "@" << std::to_string(player->getAngle()) << std::endl;
		}
		
	}

	void stop() {
		player->stop();
		shot->stop();
		delete(shot);
		delete(shotb);
	}

	void resize() {

	}
};