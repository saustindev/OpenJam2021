#pragma once
#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#define IP "127.0.0.1"

class NetworkManager {
private:
	static constexpr int port = 5555;
	sf::TcpSocket socket;
	typedef struct scoreData {
		bool dog;
		int score;
	} ScoreData;

public:
	NetworkManager() {
		sf::Socket::Status status = socket.connect(IP, port);
		if (status != sf::Socket::Done) {
			std::cout << "Failed to connect." << std::endl;
		}
	}

	void sendScore(bool dog, int score) {
		ScoreData data = { dog, score };
		if (socket.send(&data, sizeof(ScoreData)) != sf::Socket::Done)
		{
			std::cout << "Failed to send score packet." << std::endl;
		}
	}

	int getScore(bool dog) {
		char c = dog ? 1 : 0;
		int result;
		std::size_t received;
		if (socket.send(&c, 1) != sf::Socket::Done)
		{
			std::cout << "Failed to request score." << std::endl;
			return -1;
		}
		else {
			if (socket.receive(&result, sizeof(int), received) != sf::Socket::Done)
			{
				std::cout << "Failed to receive score." << std::endl;
				return -1;
			}
			else {
				return result;
			}
		}
	}
};