#pragma once
class Updateable {
public:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void click() = 0;
	virtual void stop() = 0;
};