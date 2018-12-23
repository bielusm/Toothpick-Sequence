#include <stdio.h>

#include <vector>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Toothpick.hpp"
#include "loadshaders.hpp"

using namespace glm;

#define PICKWIDTH 0.2f

#define MAX_PICKS 3
int main()
{
	std::vector<Toothpick> picks;
	picks.push_back(Toothpick(0.0f, 0.0f, false, PICKWIDTH));
	int numPicks = 1;
	size_t currPickSize = picks.size();
		for ( size_t j = 0; j < currPickSize && numPicks < MAX_PICKS; j++)
		{
			Toothpick::Dir dir = picks[j].freeSide();
			if (dir != Toothpick::Dir::none)
			{
				switch (dir)
				{
				case Toothpick::Dir::left:
					picks.push_back(Toothpick(picks[j].getLeft(), picks[j].getTop(), true, PICKWIDTH));
					break;
				case Toothpick::Dir::right:
					picks.push_back(Toothpick(picks[j].getRight(), picks[j].getTop(), true, PICKWIDTH));
					break;
				case Toothpick::Dir::top:
					picks.push_back(Toothpick(picks[j].getLeft(), picks[j].getTop(), false, PICKWIDTH));
					break;
				case Toothpick::Dir::bottom:
					picks.push_back(Toothpick(picks[j].getLeft(), picks[j].getBottom(), false, PICKWIDTH));
					break;
				}
				picks[j].setCaptured(dir);
				numPicks++;
			}
		}
	std::vector<GLfloat> g_vertex_buffer_data;
	int size = static_cast<int>(picks.size()); //so compiler doesen't complain about size_t vs int
	for (int i = 0; i < size; i++)
	{
		g_vertex_buffer_data.push_back(picks[i].getLeft());
			g_vertex_buffer_data.push_back(picks[i].getTop());
			g_vertex_buffer_data.push_back(0.0f);

			g_vertex_buffer_data.push_back(picks[i].getRight());
		g_vertex_buffer_data.push_back(picks[i].getBottom());
		g_vertex_buffer_data.push_back(0.0f);
	}

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(600, 600, "Chop Sticks", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_vertex_buffer_data.size(), &g_vertex_buffer_data.front(), GL_STATIC_DRAW);


	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	do {
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		int size = static_cast<int>(g_vertex_buffer_data.size());
		glDrawArrays(GL_LINES, 0, 6); //hard coded to 4
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}

