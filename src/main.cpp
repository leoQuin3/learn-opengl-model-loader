#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.cpp"
#include "camera.h"
#include "stb_image.h"
#include "model.h"

#ifdef PROJECT_ROOT_DIR

// Constants
const unsigned int SCREEN_WIDTH = 1080;
const unsigned int SCREEN_HEIGHT = 1080;
const std::string VERTEX_SHADER_PATH = std::string(PROJECT_ROOT_DIR) + "/assets/vert.glsl";
const std::string FRAGMENT_SHADER_PATH = std::string(PROJECT_ROOT_DIR) + "/assets/frag.glsl";
const std::string MODEL_ASSET_PATH = std::string(PROJECT_ROOT_DIR) + "/assets/backpack.obj";

// Delta time
float deltaTime = 0.f;
float lastFrame = 0.f;

// Camera
Camera camera = Camera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.f;
float lastY = SCREEN_HEIGHT / 2.f;

// Callback functions
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void cursorCallback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow* window);

// TODO:
//	- Left off at 'Advanced OpenGL > Depth Testing'
//	- Edit fragment shader to allow multiple number of texture maps (diffuse and specular)

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

	// Register callback functions
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorCallback);

	// Load functions using glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad\n";
		return EXIT_FAILURE;
	}

	// Settings
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Create shader program, camera, and model
	Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());
	Model model(MODEL_ASSET_PATH.c_str());

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Poll key input
		processInput(window);

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader program
		shader.use();

		// Camera transformations
		glm::mat4 viewMtrx = camera.getViewMatrix();
		glm::mat4 projectionMtrx = glm::perspective(glm::radians(camera.fov), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, .1f, 100.f);
		shader.setMat4("projection", projectionMtrx);
		shader.setMat4("view", viewMtrx);

		// Model transformations
		glm::mat4 modelMtrx = glm::mat4(1.f);
		modelMtrx = glm::translate(modelMtrx, glm::vec3(0.f));
		modelMtrx = glm::scale(modelMtrx, glm::vec3(1.f));
		glm::mat4 normalMtrx = glm::transpose(glm::inverse(normalMtrx));
		shader.setMat4("model", modelMtrx);
		shader.setMat4("normalModel", modelMtrx);

		// Lighting
		shader.setVec3("dirLight.direction", glm::vec3(-.2f, -1.f, -.3f));
		shader.setVec3("dirLight.ambient", glm::vec3(.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(.4f));
		shader.setVec3("dirLight.specular", glm::vec3(.5f));

		// Draw model
		model.draw(shader);

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

// Process movement and key input
void processInput(GLFWwindow* window)
{
	// Quit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// Strafing
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);

	// Sprinting
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.isSprinting = true;
	else
		camera.isSprinting = false;

	// Vertical Movement
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::DOWN, deltaTime);

	// Reset position and rotation
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		camera.position = glm::vec3(0.f, 0.f, 3.f);
		camera.lookAtPosition(glm::vec3(0.f, 0.f, -1.f));
	}
}

// Rotate camera using mouse
void cursorCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xpos = static_cast<float>(xPosIn);
	float ypos = static_cast<float>(yPosIn);

	// Avoid jerking within the first frame
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = static_cast<float>(xpos) - lastX;
	float yOffset = lastY - static_cast<float>(ypos);
	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xOffset, yOffset, true);
}

#endif