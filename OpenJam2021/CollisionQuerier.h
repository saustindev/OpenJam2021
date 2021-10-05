#pragma once
#include <string>
class CollisionQuerier {
public:
	virtual void onCollide(std::string id) = 0;
};