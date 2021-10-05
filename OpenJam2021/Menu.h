#pragma once
#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Globals.h"
#include "Updateable.h"
#include "NetworkManager.h"

class Button {
	std::string name;
	sf::RectangleShape *rect;
	sf::Text *text;
	float dims[4];
	sf::Color bgColor1;
	sf::Color bgColor2;
	sf::Color textColor1;
	sf::Color textColor2;
	static constexpr int TEXT_SIZE = 30;

public:
	Button(std::string name, float x, float y, float w, float h, sf::Color bgColor1, sf::Color bgColor2, sf::Color textColor1, sf::Color textColor2) {
		this->name = name;
		rect = new sf::RectangleShape();
		rect->setPosition({ Globals::windowX * x,Globals::windowY * y });
		rect->setSize({ Globals::windowX * w,Globals::windowY * h });
		rect->setFillColor(bgColor1);
		rect->setOutlineColor(bgColor2);
		rect->setOutlineThickness(1);
		dims[0] = x;
		dims[1] = y;
		dims[2] = w;
		dims[3] = h;
		this->bgColor1 = bgColor1;
		this->bgColor2 = bgColor2;
		this->textColor1 = textColor1;
		this->textColor2 = textColor2;
		text = new sf::Text();
		text->setFont(*Globals::font);
		text->setString(name);
		text->setCharacterSize(Button::TEXT_SIZE);
		text->setFillColor(textColor1);
		text->setPosition(Globals::windowX * dims[0] + (rect->getLocalBounds().width/2) - (text->getLocalBounds().width/2), Globals::windowY * dims[1] + (rect->getLocalBounds().height / 2) - (text->getLocalBounds().height));
	}

	void update() {
		
		if (hovered()) {
			rect->setFillColor(bgColor2);
			text->setFillColor(textColor2);
			rect->setOutlineColor(bgColor1);
		}
		else {
			rect->setFillColor(bgColor1);
			text->setFillColor(textColor1);
			rect->setOutlineColor(bgColor2);
		}
	}

	bool hovered() {
		//std::cout << "mouse: " << Globals::mouseX << "," << Globals::mouseY << " | rect: " << Globals::windowX * dims[0] << "," << Globals::windowY * dims[1] << " " << Globals::windowX * (dims[0] + dims[2]) << "x" << Globals::windowY * (dims[1] + dims[3]) <<std::endl;
		if (Globals::mouseX >= Globals::windowX * dims[0] && Globals::mouseX <= Globals::windowX * (dims[0] + dims[2]) && Globals::mouseY >= Globals::windowY * dims[1] && Globals::mouseY <= Globals::windowY * (dims[1] + dims[3])) {
			return true;
		}
		else
		{
			return false;
		}
		
	}

	void draw() {
		Globals::window->draw(*rect);
		Globals::window->draw(*text);
	}

	std::string click() {
		if (hovered()) {
			return name;
		}
		else {
			return "";
		}
	}
};

class Text {
	std::string name;
	sf::RectangleShape* rect;
	sf::Text* text;
	float dims[4];
	sf::Color textColor;

public:
	Text(std::string name, int size, float x, float y, float w, float h, sf::Color textColor) {
		this->name = name;
		rect = new sf::RectangleShape();
		rect->setPosition({ Globals::windowX * x,Globals::windowY * y });
		rect->setSize({ Globals::windowX * w,Globals::windowY * h });
		rect->setFillColor(*(new sf::Color(0,0,0,0)));
		dims[0] = x;
		dims[1] = y;
		dims[2] = w;
		dims[3] = h;
		this->textColor = textColor;
		text = new sf::Text();
		text->setFont(*Globals::font);
		text->setString(name);
		text->setCharacterSize(size);
		text->setFillColor(textColor);
		text->setPosition(Globals::windowX * dims[0] + (rect->getLocalBounds().width / 2) - (text->getLocalBounds().width / 2), Globals::windowY * dims[1] + (rect->getLocalBounds().height / 2) - (text->getLocalBounds().height));
	}

	void draw() {
		Globals::window->draw(*text);
	}

	void setString(std::string st) {
		text->setString(st);
	}

};

class Graphic {
private:
	sf::RectangleShape rect;
	float dims[4];
public:
	Graphic(float x, float y, float w, float h, std::string filename) {
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(filename);
		rect.setTexture(texture);
		dims[0] = x;
		dims[1] = y;
		dims[2] = w;
		dims[3] = h;
		rect.setPosition({ Globals::windowX * x,Globals::windowY * y });
		rect.setSize({ Globals::windowX * w,Globals::windowY * h });
	}

	void draw() {
		Globals::window->draw(rect);
	}
};

class Menu : public Updateable {
protected:
	std::vector<Button*> buttons;
	std::vector<Graphic*> graphics;
	std::vector<Text*> texts;
	std::vector<std::string> commands;
public:
	Menu() {
	}

	virtual void eventListener(std::string command) = 0;

	void update() {
		for (Button* b : buttons) {
			b->update();
		}
		while (commands.size() != 0) {
			eventListener(commands.at(commands.size() - 1));
			commands.pop_back();
		}
	}

	void draw() {
		for (Graphic* g : graphics) {
			g->draw();
		}
		for (Text* t : texts) {
			t->draw();
		}
		for (Button* b : buttons) {
			b->draw();
		}
	}

	void click() {
		for (Button* b : buttons) {
			std::string cl = b->click();
			if (cl != "") {
				commands.push_back(cl);
			}
		}
	}

	void stop() {

	}
};

class MainMenu : public Menu {
public:
	MainMenu() {
		buttons.push_back(new Button("Start", 0.6f, 0.1f, 0.2f, 0.1f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
		buttons.push_back(new Button("Scores", 0.6f, 0.25f, 0.2f, 0.1f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
		buttons.push_back(new Button("About", 0.6f, 0.40f, 0.2f, 0.1f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
		buttons.push_back(new Button("Quit", 0.6f, 0.55f, 0.2f, 0.1f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
		graphics.push_back(new Graphic(0.1f, 0.1f, 0.2f, 0.4f, "Assets/cat.png"));
		graphics.push_back(new Graphic(0.1f, 0.5f, 0.4f, 0.4f, "Assets/dog.png"));
		texts.push_back(new Text("Cats vs. Dogs:", (int)(0.1f * Globals::windowY), 0.5f, 0.65f, 0.5f, 0.2f, sf::Color::Black));
		texts.push_back(new Text("Fatally Furrious", (int)(0.05f * Globals::windowY), 0.5f, 0.85f, 0.5f, 0.1f, sf::Color::Black));
	}

	void eventListener(std::string command) {
		if (command == "Start") {
			Globals::game->swap("expo");
		}
		else if (command == "About") {
			Globals::game->swap("about");
		}
		else if (command == "Scores") {
			Globals::game->swap("score");
		}
		else if (command == "Quit") {
			Globals::game->stop();
		}
	}
};

class ExpositionMenu : public Menu {
public:
	ExpositionMenu() {
		int size = (int)(0.042f * Globals::windowY);
		graphics.push_back(new Graphic(0, 0, 1, 1, "Assets/expo.png"));
		texts.push_back(new Text("The time for peace is over. After years of failed talks,", size, 0, 0, 1, 0.12f, sf::Color::Black));
		texts.push_back(new Text("relations between cats and dogs have decayed", size, 0, 0.13f, 1, 0.12f, sf::Color::Black));
		texts.push_back(new Text("and war is inevitable.", size, 0, 0.26f, 1, 0.12f, sf::Color::Black));
		texts.push_back(new Text("The only solution to this struggle will be violence.", size, 0, 0.38f, 1, 0.12f, sf::Color::Black));
		 
		buttons.push_back(new Button("Accept my fate", 0.3f, 0.6f, 0.4f, 0.2f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));

	}

	void eventListener(std::string command) {
		if (command == "Accept my fate") {
			Globals::game->swap("choose");
		}
	}
};

class AboutMenu : public Menu {
public:
	AboutMenu() {
		int size = (int)(0.03f * Globals::windowY);
		graphics.push_back(new Graphic(0.1f, 0.55f, 0.3f, 0.3f, "Assets/cat.png"));
		texts.push_back(new Text("With the theme of this Open Jam being Cats OR Dogs,", size, 0, 0, 1, 0.12f, sf::Color::Black));
		texts.push_back(new Text("I decided to put the choice in the hands of the players.", size, 0, 0.13f, 1, 0.12f, sf::Color::Black));
		texts.push_back(new Text("The points you earn will be tallied on a server.", size, 0, 0.26f, 1, 0.12f, sf::Color::Black));
		texts.push_back(new Text("Play a lot and show the superiority of your favorite pet.", size, 0, 0.38f, 1, 0.12f, sf::Color::Black));

		buttons.push_back(new Button("Back", 0.55f, 0.6f, 0.4f, 0.2f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));

	}

	void eventListener(std::string command) {
		if (command == "Back") {
			Globals::game->swap("main");
		}
	}
};

class ChooseMenu : public Menu {
public:
	ChooseMenu() {
		int size = (int)(0.1f * Globals::windowY);
		graphics.push_back(new Graphic(0.05f, 0.4f, 0.25f, 0.25, "Assets/cat.png"));
		graphics.push_back(new Graphic(0.7f, 0.4f, 0.25f, 0.25, "Assets/dog.png"));
		texts.push_back(new Text("Choose your side:", size, 0, 0, 1, .25f, sf::Color::Black));
		buttons.push_back(new Button("Cat", 0.05f, 0.7f, 0.25f, .25f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
		buttons.push_back(new Button("Dog", 0.7f, 0.7f, 0.25f, .25f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
	}

	void eventListener(std::string command) {
		if (command == "Dog") {
			Globals::game->swap("doggame");
		}
		else if (command == "Cat") {
			Globals::game->swap("catgame");
		}
	}
};

class ScoreMenu : public Menu {
private:
	NetworkManager* net;
public:
	ScoreMenu() {
		net = Globals::net;
		int size = (int)(0.1f * Globals::windowY);

		texts.push_back(new Text("Scores:", size, 0, 0, 1, .25f, sf::Color::Black));
		texts.push_back(new Text("Cats: " + std::to_string(net->getScore(false)), size/2, 0, 0.25f, 0.5f, 0.25f, sf::Color::Black));
		texts.push_back(new Text("Dogs: " + std::to_string(net->getScore(true)), size/2, 0.5f, 0.25f, 0.5f, 0.25f, sf::Color::Black));
		buttons.push_back(new Button("Refresh", 0.05f, 0.7f, 0.25f, .25f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
		buttons.push_back(new Button("Back", 0.7f, 0.7f, 0.25f, .25f, sf::Color::White, sf::Color::Black, sf::Color::Black, sf::Color::White));
	}

	void eventListener(std::string command) {
		if (command == "Refresh") {
			texts.at(1)->setString("Cats: " + std::to_string(net->getScore(false)));
			texts.at(2)->setString("Dogs: " + std::to_string(net->getScore(true)));
		}
		else if (command == "Back") {
			Globals::game->swap("main");
		}
	}
};