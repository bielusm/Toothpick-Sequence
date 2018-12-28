#include "Toothpick.hpp"
#include "loadshaders.hpp"
#include "gfx.hpp"
#include "world.h"

#define PICKWIDTH 0.2f
#define PICK_OFFSET 0.02f
#define MAX_GEN 100
#define WORLD_SIZE 10.0f

GLFWwindow* window;
int main()
{

	Gfx gfx(&window, WORLD_SIZE);
	World world(PICKWIDTH,PICK_OFFSET, MAX_GEN);
	do {
		world.update();
		gfx.draw(world.picks, world.deadPicks,PICK_OFFSET);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
