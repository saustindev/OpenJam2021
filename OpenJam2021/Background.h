#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Globals.h"
class Background {
private:
	sf::RectangleShape* bg1;
	sf::RectangleShape* bg2;
	std::vector<sf::Sound*> sounds;
	std::vector<sf::SoundBuffer*> bufs;
	sf::Clock clock;
	static constexpr int soundCount = 15;
	static constexpr float maxInterval = 2.0f;
	static constexpr float minInterval = 0.1f;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> *distribution1;
	std::uniform_int_distribution<int>* distribution2;
	float timeTilNext;
	bool stopped;

public:
	Background() {
		stopped = false;
		sf::Texture* tx = new sf::Texture();
		bg1 = new sf::RectangleShape();
		bg1->setSize({ (float)Globals::windowX, (float)Globals::windowY });
		tx->loadFromFile("Assets/bg.png");
		bg1->setTexture(tx);
		bg2 = new sf::RectangleShape();
		
		bg2->setSize({ (float)Globals::windowX, (float)Globals::windowY });
		bg2->setOrigin((float)Globals::windowX / 2, (float)Globals::windowY / 2);
		bg2->setScale(-1, 1);
		bg2->setPosition((float)Globals::windowX * 1.5f, (float)Globals::windowY / 2);
		bg2->setTexture(tx);

		distribution1 = new std::uniform_real_distribution<float>(minInterval, maxInterval);
		distribution2 = new std::uniform_int_distribution<int>(0, soundCount - 1);
		timeTilNext = (*distribution1)(generator);

		for (int i = 0; i < soundCount; i++) {
			sounds.push_back(new sf::Sound());
			bufs.push_back(new sf::SoundBuffer());
			bufs.at(i)->loadFromFile("Assets/BG/bang (" + std::to_string(i+1) + ").ogg");
			sounds.at(i)->setBuffer(*bufs.at(i));
			sounds.at(i)->setVolume(25);
		}
	}

	void stop() {
		stopped = true;
		for (int i = 0; i < soundCount; i++) {
			sounds.at(i)->stop();
			delete(sounds.at(i));
			delete(bufs.at(i));
		}
	}

	void draw() {
		Globals::window->draw(*bg1);
		Globals::window->draw(*bg2);
	}

	void update() {
		if (stopped)
			return;
		if (clock.getElapsedTime().asSeconds() > timeTilNext) {
			clock.restart();
			int which = (*distribution2)(generator);
			sounds.at(which)->play();
		}
	}

	void move(float x, float y) {
		bg1->move(0.05f*x, y*0.25f);
		bg2->move(0.05f*x, y * 0.25f);
	}
};