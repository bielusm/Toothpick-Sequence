#include <stdio.h>

#include <vector>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Toothpick.hpp"
#include "loadshaders.hpp"

using namespace glm;

void setSides(Toothpick *pick);
bool noPerpPicks(Toothpick pick);

#define PICKWIDTH 0.2f

#define PICK_OFFSET 0.02f
#define MAX_GEN 10

std::vector<Toothpick> picks;
std::vector<Toothpick> deadPicks;
int N = 1;
int maxgen = MAX_GEN;


GLuint VertexArrayID;

GLuint programID;
GLFWwindow* window; 

int setup()
{
	picks.push_back(Toothpick(0.0f, 0.0f, false, PICKWIDTH));
	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 0); // 0x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	window = glfwCreateWindow(1280, 720, "Chop Sticks", NULL, NULL);
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
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//allowedPicks = 1;
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	return 0;
}

void update()
{
	if(N < maxgen)
	{ 
		int size = picks.size();
		for (size_t j = 0; j < size; j++)
		{
			Toothpick::Dir dir = picks[j].freeSide();
			while (dir != Toothpick::Dir::none)
			{
				Toothpick temp;
				switch (dir)
				{
				case Toothpick::Dir::left:
					temp = Toothpick(picks[j].getLeft() - PICK_OFFSET, picks[j].getTop(), true, PICKWIDTH);
					break;
				case Toothpick::Dir::right:
					temp = Toothpick(picks[j].getRight() + PICK_OFFSET, picks[j].getTop(), true, PICKWIDTH);
					break;
				case Toothpick::Dir::top:
					temp = Toothpick(picks[j].getLeft(), picks[j].getTop() + PICK_OFFSET, false, PICKWIDTH);
					break;
				case Toothpick::Dir::bottom:
					temp = Toothpick(picks[j].getLeft(), picks[j].getBottom() - PICK_OFFSET, false, PICKWIDTH);
					break;
				}
				picks[j].setCaptured(dir);
				dir = picks[j].freeSide();
				//if (noPerpPicks(temp))
				{
					picks.push_back(temp);
					setSides(&picks[picks.size() - 1]);
				}
			}
			//deadPicks.push_back(picks[j]);
		//	picks.erase(picks.begin()); //very slow consider a queue
			N++;
		}
	}


}

void draw()
{	
	glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f);
	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);
	//glm::mat4 Model = glm::mat4(1.0f);
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
		xOffset = (facingUp) ? 0 : PICK_OFFSET;
		yOffset = (facingUp) ? PICK_OFFSET : 0;

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
		bool facingUp = picks[i].getFacingUp();
		xOffset = (facingUp) ? 0 : PICK_OFFSET;
		yOffset = (facingUp) ? PICK_OFFSET : 0;

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


//	if (allowedPicks < MAX_PICKS)
	//	allowedPicks++;

	glfwSwapBuffers(window);
}

int main()
{
	setup();

	do {
		update();
		draw();
		glfwPollEvents();
		int newState = glfwGetKey(window, GLFW_KEY_SPACE);
		//if(newState == GLFW_PRESS )
			maxgen++;
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}

void setSides(Toothpick *pick)
{
	if (pick->getFacingUp())
	{
		float top = pick->getTop();
		float bottom = pick->getBottom();
		float plane = pick->getLeft();
		float top2, bottom2, plane2;
		for (int i = 0; i < picks.size(); i++)
		{
			top2 = picks[i].getTop();
			bottom2 = picks[i].getBottom();
			plane2 = picks[i].getLeft();
			bool samePlane = (abs(plane - plane2) < 0.1);
			bool topEqual = (abs(top2 - bottom) < 0.1);
			bool bottomEqual = (abs(bottom2 - top) < 0.1);
			if (samePlane && (topEqual || bottomEqual))
			{
				if (!topEqual)
				{
					picks[i].setCaptured(3);
					pick->setCaptured(2);
				}
				else
				{
					picks[i].setCaptured(2);
					pick->setCaptured(3);

				}

			}
		}
	}
	else
	{
		float left = pick->getLeft();
		float right = pick->getRight();
		float plane = pick->getTop();
		float left2, right2, plane2;
		for (int i = 0; i < picks.size(); i++)
		{
			left2 = picks[i].getLeft();
			right2 = picks[i].getRight();
			plane2 = picks[i].getTop();
			bool samePlane = (abs(plane - plane2) < 0.1);
			bool leftEqual = (abs(left2 - right) < 0.1);
			bool rightEqual = (abs(right2 - left) < 0.1);
			if (samePlane &&(leftEqual || rightEqual))
			{
				if (leftEqual)
				{
					picks[i].setCaptured(0);
					pick->setCaptured(1);
				}
				else
				{
					picks[i].setCaptured(1);
					pick->setCaptured(0);
				}
			}
		}
	}
}

bool noPerpPicks(Toothpick pick)
{
	bool facingUp = pick.getFacingUp();
	float top, bottom, left, right, x, y;
	float top2, bottom2, left2, right2,x2,y2;
	top = pick.getTop();
	bottom = pick.getBottom();
	left = pick.getLeft();
	right = pick.getRight();
	x = pick.getX();
	y = pick.getY();
	for (int i = 0; i < picks.size(); i++)
	{
		bool pickFacingUp = picks[i].getFacingUp();
		if (facingUp != pickFacingUp)
		{
			top2 = picks[i].getTop();
			bottom2 = picks[i].getBottom();
			left2 = picks[i].getLeft();
			right2 = picks[i].getRight();
			x2 = picks[i].getX();
			y2 = picks[i].getY();
			if (facingUp)
			{
				if (abs(x - x2) < 0.1)
				{
					if ((abs(top - top2) < 0.1) || (abs(bottom - bottom2) < 0.1))
						return false;
				}
			}
			else
			{
				if (abs(y - y2) < 0.1)
				{
					if ((abs(left - left2) < 0.1) || (abs(right - right2) < 0.1))
						return false;
				}
			}
		}
	}
	return true;
}