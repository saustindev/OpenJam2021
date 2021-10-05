#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class PlayerUI {
private:
	static constexpr float maxHP = 100;
	static constexpr unsigned int size = 40;
	float curHP;
	int points;
	sf::RectangleShape* bottomHealth;
	sf::RectangleShape* topHealth;
	sf::Text* pointText;
public:
	PlayerUI() {
		curHP = maxHP;
		points = 0;
		bottomHealth = new sf::RectangleShape();
		topHealth = new sf::RectangleShape();
		bottomHealth->setFillColor(sf::Color::Red);
		topHealth->setFillColor(sf::Color::Green);
		bottomHealth->setOutlineColor(sf::Color::Black);
		topHealth->setOutlineColor(sf::Color::Black);
		bottomHealth->setOutlineThickness(1);
		topHealth->setOutlineThickness(1);

		bottomHealth->setPosition(0.05f * Globals::windowX, 0.85f * Globals::windowY);
		bottomHealth->setSize({ 0.4f * Globals::windowX, 0.1f * Globals::windowY });
		topHealth->setPosition(0.05f * Globals::windowX, 0.85f * Globals::windowY);
		topHealth->setSize({ 0.4f * Globals::windowX * curHP / maxHP, 0.1f * Globals::windowY });

		pointText = new sf::Text();
		pointText->setFont(*Globals::font);
		pointText->setCharacterSize(size);
		pointText->setFillColor(sf::Color::Black);
		pointText->setString(std::to_string(points));
		pointText->setOrigin(pointText->getLocalBounds().width / 2, pointText->getLocalBounds().height / 2);
		pointText->setPosition(0.75f * Globals::windowX, 0.90f * Globals::windowY);
	}

	void addPoints(int points) {
		this->points += points;
		pointText->setString(std::to_string(this->points));
		pointText->setOrigin(pointText->getLocalBounds().width / 2, pointText->getLocalBounds().height / 2);
		pointText->setPosition(0.75f * Globals::windowX, 0.90f * Globals::windowY);
	}

	void resetPoints() {
		points = 0;
		pointText->setString(std::to_string(points));
		pointText->setOrigin(pointText->getLocalBounds().width / 2, pointText->getLocalBounds().height / 2);
		pointText->setPosition(0.75f * Globals::windowX, 0.90f * Globals::windowY);
	}

	int getPoints() {
		return points;
	}

	void hurt(float dmg) {
		curHP -= dmg;
		topHealth->setSize({ 0.4f * Globals::windowX * curHP / maxHP, 0.1f * Globals::windowY });
	}

	void resetHP() {
		curHP = maxHP;
		topHealth->setSize({ 0.4f * Globals::windowX * curHP / maxHP, 0.1f * Globals::windowY });
	}

	void draw() {
		Globals::window->draw(*bottomHealth);
		Globals::window->draw(*topHealth);
		Globals::window->draw(*pointText);
	}

	bool isDead() {
		return curHP <= 0;
	}
};