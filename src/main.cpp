#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCREEN_WIDTH = 1080;
const unsigned int SCREEN_HEIGHT = 1080;

// Prototypes
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	// Initialize glfw
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw.\n";
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Model Loader", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create glfw window.\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	// Register functions
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Load functions using glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad\n";
		return EXIT_FAILURE;
	}

	// Rest of the code here...

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
