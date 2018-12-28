#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>
#include "loadShaders.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include "Toothpick.hpp"

using namespace glm;

Gfx::Gfx(GLFWwindow** window, float worldSize)
{
	this->window = window;
	this->worldSize = worldSize;

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 0); // 0x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
															   // Open a window and create its OpenGL context
	*window = glfwCreateWindow(1280, 720, "Toothpicks", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(*window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(*window, GLFW_STICKY_KEYS, 1);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
}

void Gfx::draw(std::vector<Toothpick> picks, std::vector<Toothpick>deadPicks, float pickOffset)
{
	glm::mat4 Projection = glm::ortho(-worldSize, worldSize, -worldSize, worldSize, 0.0f, worldSize);
	glm::mat4 mvp = Projection;
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUseProgram(programID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float xOffset, yOffset;
	std::vector<GLfloat> g_vertex_buffer_data;
	int size = static_cast<int>(picks.size());
	for (int i = 0; i < size; i++)
	{
		bool facingUp = picks[i].getFacingUp();
		xOffset = (facingUp) ? 0 : pickOffset;
		yOffset = (facingUp) ? pickOffset : 0;

		g_vertex_buffer_data.push_back(picks[i].getLeft() + xOffset);
		g_vertex_buffer_data.push_back(picks[i].getTop() - yOffset);
		g_vertex_buffer_data.push_back(0.0f);

		g_vertex_buffer_data.push_back(picks[i].getRight() - xOffset);
		g_vertex_buffer_data.push_back(picks[i].getBottom() + yOffset);
		g_vertex_buffer_data.push_back(0.0f);
	}
	size = static_cast<int>(deadPicks.size());
	for (int i = 0; i < size; i++)
	{
		bool facingUp = deadPicks[i].getFacingUp();
		xOffset = (facingUp) ? 0 : pickOffset;
		yOffset = (facingUp) ? pickOffset : 0;

		g_vertex_buffer_data.push_back(deadPicks[i].getLeft() + xOffset);
		g_vertex_buffer_data.push_back(deadPicks[i].getTop() - yOffset);
		g_vertex_buffer_data.push_back(0.0f);

		g_vertex_buffer_data.push_back(deadPicks[i].getRight() - xOffset);
		g_vertex_buffer_data.push_back(deadPicks[i].getBottom() + yOffset);
		g_vertex_buffer_data.push_back(0.0f);
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_vertex_buffer_data.size(), &g_vertex_buffer_data.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glVertexPointer(3, GL_FLOAT, g_vertex_buffer_data.size() * sizeof(GLfloat), &vbo);
	glDrawArrays(GL_LINES, 0, g_vertex_buffer_data.size());

	glfwSwapBuffers(*window);
}