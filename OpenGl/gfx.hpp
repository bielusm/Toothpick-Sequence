#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include "Toothpick.hpp"
class Gfx
{
public:
	Gfx(GLFWwindow ** window, float worldSize);
	void draw(std::vector<Toothpick> picks, std::vector<Toothpick> deadPicks, float pickOffset);
private:
	float worldSize;
	GLFWwindow ** window;
	GLuint VertexArrayID;
	GLuint programID;
};

