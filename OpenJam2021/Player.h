#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Globals.h"
#include "Collision.h"
#include "CollisionQuerier.h"
#include "Background.h"
#include "PlayerUI.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "NetworkManager.h"

class TextBox {
private:
	sf::RectangleShape* box;
	sf::RectangleShape* img;
	
	std::string text;
	std::string name;
	sf::Text *nameTx;
	sf::Text *tx;
	static constexpr int fontSize = 15;
public:
	TextBox(std::string name, std::string text, std::string imgPath) {
		box = new sf::RectangleShape();
		box->setPosition({ 0.1f * Globals::windowX, 0.05f * Globals::windowY });
		box->setSize({ 0.8f * Globals::windowX,0.2f * Globals::windowY });
		box->setFillColor(sf::Color::White);
		box->setOutlineColor(sf::Color::Black);
		box->setOutlineThickness(1);
		sf::Texture *tex = new sf::Texture();
		tex->loadFromFile(imgPath);
		img = new sf::RectangleShape();
		img->setTexture(tex);
		img->setPosition({ 0.12f * Globals::windowX, 0.07f * Globals::windowY });
		img->setSize({ 0.1067f * Globals::windowX, 0.16f * Globals::windowY});
		img->setOutlineColor(sf::Color::Black);
		img->setOutlineThickness(1);

		nameTx = new sf::Text();
		nameTx->setFont(*Globals::font);
		nameTx->setCharacterSize((unsigned int)(fontSize * 1.5f));
		nameTx->setPosition({ 0.235f * Globals::windowX, 0.07f * Globals::windowY });
		nameTx->setString(name);
		nameTx->setFillColor(sf::Color::Black);

		tx = new sf::Text();
		tx->setFont(*Globals::font);
		tx->setCharacterSize(fontSize);
		tx->setPosition({ 0.235f * Globals::windowX, 0.12f * Globals::windowY });
		tx->setString(text);
		tx->setFillColor(sf::Color::Black);
	}

	void draw() {
		Globals::window->draw(*box);
		Globals::window->draw(*img);
		Globals::window->draw(*nameTx);
		Globals::window->draw(*tx);
	}

	void setBox(std::string n, std::string t, std::string f) {
		tx->setString(t);
		nameTx->setString(n);
		sf::Texture* newtx = new sf::Texture();
		newtx->loadFromFile(f);
		img->setTexture(newtx);
		img->setTextureRect({ 0, 0, (int)newtx->getSize().x, (int)newtx->getSize().y });
	}
};

class Player : public CollisionQuerier {
private:
	TextBox* box;
	Background* bg;
	PlayerUI* ui;
	EnemyManager* en;
	BulletManager* bul;
	NetworkManager* net;
	sf::Sound* die;
	sf::SoundBuffer* dieBuf;
	sf::Sound* pain;
	sf::SoundBuffer* painBuf;
	int origX;
	int origY;

	bool dog;
	sf::RectangleShape* body;
	sf::RectangleShape* arm;
	sf::RectangleShape* reference;
	std::vector<sf::Texture*> bodyTex;
	int texNum;
	static constexpr int w = 300;
	static constexpr int h = 200;
	static constexpr float grav = 1500;
	static constexpr float jumpVel = 1000;
	static constexpr float dDefSpawnX = 700;
	static constexpr float dDefSpawnY = 480;
	static constexpr float cDefSpawnX = 21800;
	static constexpr float cDefSpawnY = -380;
	float spawnX;
	float spawnY;
	float yVel;
	double angle;
	bool flipped;
	sf::Clock clock;
	sf::Clock animClock;

	enum class STATE : int {
		Left = 0,
		Still = 1,
		Aerial = 2,
		Right = 3,
		Cutscene = 4
	};

	STATE curState;

	bool lastState[128] = { 0 };
	bool currentState[128] = { 0 };

	void updateKeystate() {
		//Manage keystate
		memcpy(lastState, currentState, 128);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			currentState['a'] = true;
		}
		else {
			currentState['a'] = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			currentState['d'] = true;
		}
		else {
			currentState['d'] = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			currentState[' '] = true;
		}
		else {
			currentState[' '] = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			Globals::game->swap("main");
		}
	}

	bool leftDown() {
		return (currentState['a'] && !lastState['a']);
	}
	bool leftUp() {
		return (!currentState['a'] && lastState['a']);
	}

	bool rightDown() {
		return (currentState['d'] && !lastState['d']);
	}
	bool rightUp() {
		return (!currentState['d'] && lastState['d']);
	}

	bool jumpDown() {
		return (currentState[' '] && !lastState[' ']);
	}
	bool jumpUp() {
		return (!currentState[' '] && lastState[' ']);
	}

	void updateAngle() {
		//Get angle between position and mouse, then update rotation of arm to match
		sf::Vector2f mouse = Globals::window->mapPixelToCoords({Globals::mouseX, Globals::mouseY});
		double dy = (double)mouse.y - (double)body->getPosition().y;
		double dx = (double)mouse.x - (double)body->getPosition().x;
		double angleRad = atan(dy / dx);
		angle = angleRad * 180 / M_PI;
		if (dx < 0)
			angle -= 180;
		if (flipped)
			angle -= 180;
		arm->setRotation((float)angle);
	}

	CollisionManager* col;
	std::string lastId;

public:
	Player(bool dog, CollisionManager *col) {
		bul = new BulletManager();
		bg = new Background();
		ui = new PlayerUI();
		en = new EnemyManager(dog, col, bul, ui);
		net = Globals::net;
		this->col = col;
		box = new TextBox("Joe", "Im dog", "Assets/dog.png");
		curState = STATE::Aerial;
		spawnX = dDefSpawnX;
		spawnY = dDefSpawnY;
		yVel = 0;
		origX = Globals::windowX;
		origY = Globals::windowY;
		angle = 90;
		flipped = false;
		this->dog = dog;
		body = new sf::RectangleShape();
		body->setSize({ w, h });
		body->setOrigin({ w / 2, h / 2 });
		body->setPosition({ (float)Globals::windowX / 2, (float)Globals::windowY * 0.6f });
		arm = new sf::RectangleShape();
		arm->setSize({ w, w });
		arm->setOrigin({ w / 2, w / 2 });
		arm->setPosition({ (float)Globals::windowX / 2, (float)Globals::windowY * 0.6f });
		texNum = 0;
		die = new sf::Sound();
		dieBuf = new sf::SoundBuffer();
		pain = new sf::Sound();
		painBuf = new sf::SoundBuffer();
		if (dog) {
			dieBuf->loadFromFile("Assets/dog.ogg");
			bodyTex.push_back(new sf::Texture());
			bodyTex.at(0)->loadFromFile("Assets/dog1.png");
			bodyTex.push_back(new sf::Texture());
			bodyTex.at(1)->loadFromFile("Assets/dog2.png");
			bodyTex.push_back(new sf::Texture());
			bodyTex.at(2)->loadFromFile("Assets/dogair.png");
			body->setTexture(bodyTex.at(0));
			body->setTextureRect({ 0,50,300,200 });
			sf::Texture *armTex = new sf::Texture();
			armTex->loadFromFile("Assets/dogarm.png");
			arm->setTexture(armTex);
		}
		else {
			dieBuf->loadFromFile("Assets/cat.ogg");
			bodyTex.push_back(new sf::Texture());
			bodyTex.at(0)->loadFromFile("Assets/cat1.png");
			bodyTex.push_back(new sf::Texture());
			bodyTex.at(1)->loadFromFile("Assets/cat2.png");
			bodyTex.push_back(new sf::Texture());
			bodyTex.at(2)->loadFromFile("Assets/catair.png");
			body->setTexture(bodyTex.at(0));
			body->setTextureRect({ 0,50,300,200 });
			sf::Texture* armTex = new sf::Texture();
			armTex->loadFromFile("Assets/dogarm.png");
			arm->setTexture(armTex);
		}
		die->setBuffer(*dieBuf);
		painBuf->loadFromFile("Assets/hit.ogg");
		pain->setBuffer(*painBuf);

		reference = new sf::RectangleShape();
		reference->setPosition(spawnX, spawnY);

		if (!dog) {
			spawnX = -1*cDefSpawnX;
			spawnY = -1*cDefSpawnY;
			respawn();
		}
	}

	void update() {
		bg->update();
		en->update();
		bul->update();
		updateKeystate();
		updateAngle();
		float dt = clock.restart().asSeconds();
		bool l = false;
		bool r = false;
		std::vector<std::string> collided;
		std::vector<std::string> colBuddy;
		switch (curState) {
			case STATE::Left:
				if (animClock.getElapsedTime().asMilliseconds() > 250) {
					animClock.restart();
					texNum += 1;
					if (texNum > 1)
						texNum = 0;
					body->setTexture(bodyTex.at(texNum));
				}
				move(dt * -250, 0);
				colBuddy = col->collides(body->getGlobalBounds());
				if (colBuddy.size()) {
					for (std::string s : colBuddy) {
						collided.push_back(s);
					}
					move(dt * 250, 0);
					curState = STATE::Still;
				}
				if (leftUp()) {
					curState = STATE::Still;
				}
				if (rightDown()) {
					body->setScale({ 1.0f, 1.f });
					arm->setScale({ 1.0f, 1.f });
					flipped = false;
					curState = STATE::Right;
				}
				if (jumpDown()) {
					body->setTexture(bodyTex.at(2));
					yVel = jumpVel;
					curState = STATE::Aerial;
				}
				//check if fell off
				body->move({ 0, 1 });
				colBuddy = col->collides(body->getGlobalBounds());
				if (colBuddy.size()) {
					for (std::string s : colBuddy) {
						collided.push_back(s);
					}
				}
				else {
					body->setTexture(bodyTex.at(2));
					curState = STATE::Aerial;
				}
				body->move({ 0, -1 });
				break;
			case STATE::Right:
				if (animClock.getElapsedTime().asMilliseconds() > 250) {
					animClock.restart();
					texNum += 1;
					if (texNum > 1)
						texNum = 0;
					body->setTexture(bodyTex.at(texNum));
				}
				move(dt * 250, 0);
				colBuddy = col->collides(body->getGlobalBounds());
				if (colBuddy.size()) {
					for (std::string s : colBuddy) {
						collided.push_back(s);
					}
					move(dt * -250, 0);
					curState = STATE::Still;
				}
				if (rightUp()) {
					curState = STATE::Still;
				}
				if (leftDown()) {
					body->setScale({ -1.0f, 1.f });
					arm->setScale({ -1.0f, 1.f });
					flipped = true;
					curState = STATE::Left;
				}
				if (jumpDown()) {
					body->setTexture(bodyTex.at(2));
					yVel = jumpVel;
					curState = STATE::Aerial;
				}
				//check if fell off
				body->move({ 0, 1 });
				colBuddy = col->collides(body->getGlobalBounds());
				if (colBuddy.size()) {
					for (std::string s : colBuddy) {
						collided.push_back(s);
					}
				}
				else {
					body->setTexture(bodyTex.at(2));
					curState = STATE::Aerial;
				}
				body->move({ 0, -1 });
				break;
			case STATE::Still:
				if (leftDown()) {
					body->setScale({ -1.0f, 1.f });
					arm->setScale({ -1.0f, 1.f });
					flipped = true;
					curState = STATE::Left;
				}
				else if (rightDown()) {
					body->setScale({ 1.0f, 1.f });
					arm->setScale({ 1.0f, 1.f });
					flipped = false;
					curState = STATE::Right;
				}
				if (jumpDown()) {
					body->setTexture(bodyTex.at(2));
					yVel = jumpVel;
					curState = STATE::Aerial;
				}
				break;
			case STATE::Aerial:
				if (currentState['a'] && !currentState['d']) {
					move(dt * -250, 0);
					l = true;
					colBuddy = col->collides(body->getGlobalBounds());
					if (colBuddy.size()) {
						for (std::string s : colBuddy) {
							collided.push_back(s);
						}
						move(dt * 250, 0);
						l = false;
					}
				}
				if (currentState['d'] && !currentState['a']) {
					move(dt * 250, 0);
					r = true;
					colBuddy = col->collides(body->getGlobalBounds());
					if (colBuddy.size()) {
						for (std::string s : colBuddy) {
							collided.push_back(s);
						}
						move(dt * -250, 0);
						r = false;
					}
				}
				move(0, dt* yVel * -1);
				colBuddy = col->collides(body->getGlobalBounds());
				if (colBuddy.size()) {
					for (std::string s : colBuddy) {
						collided.push_back(s);
					}
					move(0, dt* yVel);
					if (yVel < 0) {
						if (l) {
							body->setScale({ -1.0f, 1.f });
							arm->setScale({ -1.0f, 1.f });
							flipped = true;
							curState = STATE::Left;
						}
						else if (r) {
							body->setScale({ 1.0f, 1.f });
							arm->setScale({ 1.0f, 1.f });
							flipped = false;
							curState = STATE::Right;
						}
						else {
							body->setTexture(bodyTex.at(0));
							curState = STATE::Still;
						}
						yVel = 0;
					}
					else {
						//Bounce down when you bonk
						yVel *= -1;
					}
				}
				else {
					yVel -= grav * dt;
				}
				break;
			case STATE::Cutscene:
				break;
		}
		for (std::string s : collided) {
			onCollide(s);
		}
		if (bul->collides(true, body->getGlobalBounds())) {
			damage(10);
		}
		//std::cout << std::to_string(reference->getPosition().x) << ", " << std::to_string(reference->getPosition().y) << std::endl;
	}

	void stop() {
		bg->stop();
		pain->stop();
		die->stop();
		en->stop();
		delete(pain);
		delete(die);
		delete(dieBuf);
		delete(painBuf);
	}

	void move(float x, float y) {
		//body->move(x, y);
		//arm->move(x, y);
		col->move(-x, -y);
		bg->move(-x, -y);
		reference->move(-x, -y);
		en->move(-x, -y);
		bul->move(-x, -y);
	}

	double getAngle() {
		return flipped ? angle - 180 : angle;
	}

	sf::Vector2f getPosition() {
		return {body->getGlobalBounds().left + body->getOrigin().x + (float)(cos(getAngle()*M_PI/180)*150), body->getGlobalBounds().top + body->getOrigin().y + (float)(sin(getAngle() * M_PI / 180)*150)};
	}

	void onCollide(std::string id) {
		if (id == lastId)
			return;
		if (id == "tutorialdog") {
			lastId = id;
			if(dog)
				box->setBox("Dog", "Welcome to the killing fields, pup.", "Assets/dog.png");
			else
				box->setBox("Dog", "Uh oh, how'd you get in here? Don't shoot!", "Assets/dog.png");
			curState = STATE::Cutscene;
		}
		else if (id == "wiredog") {
			lastId = id;
			if (dog)
				box->setBox("Injured Dog", "These wires are nasty, dog. My paws are messed up.", "Assets/dog.png");
			else
				box->setBox("Injured Dog", "These wires are nasty, cat. My paws are messed up.", "Assets/dog.png");
			curState = STATE::Cutscene;
		}
		else if (id == "prisondog") {
			lastId = id;
			if (dog)
				box->setBox("Imprisoned Dog", "Thanks for busting me out, dog.", "Assets/dog.png");
			else
				box->setBox("Imprisoned Dog", "It was nice being free while it lasted...", "Assets/dog.png");
			curState = STATE::Cutscene;
		}
		else if (id == "camp") {
			lastId = id;
			box->setBox("System", "Checkpoint reached", "Assets/floppy.png");
			curState = STATE::Cutscene;
			spawnX = reference->getPosition().x;
			spawnY = reference->getPosition().y;
		}
		else if (id == "lazycat") {
			lastId = id;
			box->setBox("Lazy Cat", "I'm gonna need a cat nap before I jump over these gaps... *yawn*", "Assets/cat.png");
			curState = STATE::Cutscene;
		}
		else if (id == "killbox") {
			lastId = id;
			damage(100);
		}
		else if (id == "barbedwire") {
			lastId = id;
			damage(10);
			yVel = 800;
			curState = STATE::Aerial;
		}
		else if (id == "snax1") {
			lastId = id;
			ui->addPoints(250);
			ui->resetHP();
			col->remove(id);
		}
		else if (id == "snax2") {
			lastId = id;
			ui->addPoints(250);
			ui->resetHP();
			col->remove(id);
		}
		else if (id == "catnipcat") {
			lastId = id;
			if(dog)
				box->setBox("Catnip Cat", "I'll never tell you where I hid my catnip! Never!", "Assets/cat.png");
			else 
				box->setBox("Catnip Cat", "I got shot in the leg and can't jump to my catnip... How will I go on?", "Assets/cat.png");
			curState = STATE::Cutscene;
		}
		else if (id == "tutorialcat") {
			lastId = id;
			if (dog)
				box->setBox("Cat", "A dog in the base? It's all over...", "Assets/cat.png");
			else
				box->setBox("Cat", "Go get 'em, kitten. We'll show 'em they barked up the wrong tree!", "Assets/cat.png");
			curState = STATE::Cutscene;
		}
		else if (id == "ammodog") {
			lastId = id;
			if(dog)
				box->setBox("Resupply Dog", "Howdy dog! Take some ammo, their camp is right up ahead!", "Assets/dog.png");
			else
				box->setBox("Resupply Dog", "OK cat, I surrender. You can take this ammo.", "Assets/dog.png");
			curState = STATE::Cutscene;
		}
		else if (id == "nuisancecrate") {
			//you can touch a dog and this crate at the same time which is problematic
		}
		else if (id == "dogcamp") {
			lastId = id;
			if (dog) {
				box->setBox("System", "Checkpoint reached", "Assets/floppy.png");
				curState = STATE::Cutscene;
				spawnX = reference->getPosition().x;
				spawnY = reference->getPosition().y;
			}
			else {
				//Win
				ui->addPoints(2000);
				box->setBox("System", "Congratulations, you won the game! Your score is being uploaded.", "Assets/floppy.png");
				curState = STATE::Cutscene;
				respawn();
				ui->resetHP();
			}
		}
		else if (id == "catcamp") {
			lastId = id;
			if (!dog) {
				box->setBox("System", "Checkpoint reached", "Assets/floppy.png");
				curState = STATE::Cutscene;
				spawnX = reference->getPosition().x;
				spawnY = reference->getPosition().y;
			}
			else {
				//Win
				ui->addPoints(2000);
				box->setBox("System", "Congratulations, you won the game! Your score is being uploaded.", "Assets/floppy.png");
				curState = STATE::Cutscene;
				respawn();
				ui->resetHP();
				ui->resetPoints();
			}
		}
		else {
			lastId = "";
		}
	}

	void scoreUpload() {
		net->sendScore(dog, ui->getPoints());
	}

	void makeBullet() {
		bul->createBullet(true, getPosition().x, getPosition().y, getAngle());
	}

	void respawn() {
		scoreUpload();
		move(reference->getPosition().x - spawnX, reference->getPosition().y - spawnY);
		curState = STATE::Aerial;
		lastId = "";
		en->revive();
		col->restore();
	}

	bool click() {
		bool result = curState == STATE::Cutscene;
		if (result)
			curState = STATE::Aerial;
		return result;
	}

	void damage(float dmg) {
		ui->hurt(dmg);
		if (ui->isDead()) {
			respawn();
			ui->resetHP();
			ui->resetPoints();
			
			die->play();
		}
		else {
			pain->play();
		}
	}

	void draw() { 
		bg->draw();
		col->draw();
		
		bul->draw();
		en->draw();
		Globals::window->draw(*body);
		Globals::window->draw(*arm);
		if(curState == STATE::Cutscene)
			box->draw();
		ui->draw();
		
	}
};