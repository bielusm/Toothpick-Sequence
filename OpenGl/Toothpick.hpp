#pragma once
class Toothpick
{
private: 
	float x, y;
	bool facingUp;
	bool leftFree, rightFree, bottomFree, topFree = true;
	bool noneFree;
	float width;
public: 
	Toothpick(float x, float y, bool facingUp, float width);
	Toothpick();
	bool getFacingUp();
	float getX();
	float getY();
	float getLeft();
	float getRight();
	float getTop();
	float getBottom();

	enum Dir
	{
		left = 0,
		right = 1,
		bottom = 2,
		top = 3,
		none = -1
	};
	Dir freeSide();
	void setCaptured(int dir);
	void setCaptured(Dir dir);
};