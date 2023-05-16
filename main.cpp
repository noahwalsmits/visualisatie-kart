#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "stb_image.h"

#include "rendering/Shader.h"
#include "rendering/model.h"
#include "rendering/animation/AnimatedModel.h"
#include "rendering/Camera.h"
#include "gamelogic/PlayerCharacter.h"

GLFWwindow* window;
glm::ivec2 screenSize;
double lastTime;
double lastMouseX;
double lastMouseY;

const int CHARACTER_COUNT = 1;
std::vector<PlayerCharacter*> characters;
PlayerCharacter* playerCharacter;
Camera* camera;

Shader* staticShader;
std::vector<Model*> staticModels;
Shader* animatedShader;
std::vector<AnimatedModel*> animatedModels;

//rendering constants
const float FOV = 80.0f;
const float RENDER_DISTANCE = 350.0f;
const glm::vec3 SKY_COLOR = glm::vec3(0.0f, 0.0f, 0.0f);

//lighting values
float ambientLightingStrength = 0.3f;
glm::vec3 diffuseLightPosition = glm::vec3(0.0f, 1000.0f, 0.0f);
glm::vec3 lightingColor = glm::vec3(0.6f, 0.6f, 1.0f);

//uniform names
static const std::string UNIFORM_AMBIENT_STRENGTH = "ambientStrength";
static const std::string UNIFORM_LIGHT_POSITION = "lightPosition";
static const std::string UNIFORM_LIGHT_COLOR = "lightColor";
static const std::string UNIFORM_PROJECTION_MATRIX = "projectionMatrix";
static const std::string UNIFORM_VIEW_MATRIX = "viewMatrix";
static const std::string UNIFORM_MODEL_MATRIX = "modelMatrix";


#ifdef _WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}
static void glfw_error_callback(int error, const char* description)
{
	std::cerr << "Glfw Error" << error << " -> " << description << std::endl;
}


void init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b, 1.0f);

	assert(CHARACTER_COUNT > 0);
	for (int i = 0; i < CHARACTER_COUNT; i++)
	{
		characters.push_back(new PlayerCharacter(glm::vec3(1.0f * i, 0.0f, 0.0f)));
		characters[i]->registerModels(staticModels, animatedModels);
	}
	playerCharacter = characters[0];
	camera = new Camera(playerCharacter->getCameraTarget());

	staticModels.push_back(new Model("assets/arena_stage/stage.obj", glm::vec3(0.0f, -37.5f, 0.0f)));

	staticShader = new Shader("model.vs", "model.fs");
	animatedShader = new Shader("model_animated.vs", "model.fs");

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT);
	}

	lastTime = glfwGetTime();

	//setup mouse movement
	glfwGetWindowSize(window, &screenSize.x, &screenSize.y);
	lastMouseX = screenSize.x / 2;
	lastMouseY = screenSize.y / 2;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::cout << "completed init" << std::endl;
}


void display()
{
	glfwGetWindowSize(window, &screenSize.x, &screenSize.y);
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//prepare uniform data
	glm::mat4 projection = glm::perspective(glm::radians(FOV), screenSize.x / (float)screenSize.y, 0.01f, RENDER_DISTANCE);
	glm::mat4 view = camera->getViewMatrix();

	//apply to static shader and draw static models
	staticShader->use();
	staticShader->setFloat(UNIFORM_AMBIENT_STRENGTH, ambientLightingStrength);
	staticShader->setVec3(UNIFORM_LIGHT_POSITION, diffuseLightPosition);
	staticShader->setVec3(UNIFORM_LIGHT_COLOR, lightingColor);
	staticShader->setMat4(UNIFORM_PROJECTION_MATRIX, projection);
	staticShader->setMat4(UNIFORM_VIEW_MATRIX, view);
	for (Model* model : staticModels)
	{
		model->draw(*staticShader);
	}

	//apply to animated shader and draw animated models
	animatedShader->use();
	animatedShader->setFloat(UNIFORM_AMBIENT_STRENGTH, ambientLightingStrength);
	animatedShader->setVec3(UNIFORM_LIGHT_POSITION, diffuseLightPosition);
	animatedShader->setVec3(UNIFORM_LIGHT_COLOR, lightingColor);
	animatedShader->setMat4(UNIFORM_PROJECTION_MATRIX, projection);
	animatedShader->setMat4(UNIFORM_VIEW_MATRIX, view);
	for (AnimatedModel* model : animatedModels)
	{
		model->draw(*animatedShader);
	}

	glfwSwapBuffers(window);
}


void update()
{
	double time = glfwGetTime();
	double elapsed = time - lastTime;
	lastTime = time;

	//update characters
	//camera tracks player rotation while still allowing mouse adjustment
	float oldPlayerRotation = playerCharacter->getRotation();
	for (PlayerCharacter* character : characters)
	{
		character->update(elapsed, window);
	}
	camera->setYaw(camera->getYaw() - (playerCharacter->getRotation() - oldPlayerRotation));

	//update animations
	for (AnimatedModel* model : animatedModels)
	{
		model->update(elapsed);
	}

	//process keyboard input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	camera->pollMovementKeys(elapsed, window);

	//poll mouse position
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	camera->mouseMoved(mouseX - lastMouseX, mouseY - lastMouseY);
	lastMouseX = mouseX;
	lastMouseY = mouseY;
}


int main(int argc, char* argv[])
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1280, 720, "Visualisatie Kart", NULL, NULL);

	if (window == nullptr)
		return false;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	init();

	while (true)
	{
		if (glfwWindowShouldClose(window))
			break;
		glfwPollEvents();

		update();
		display();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	//clean up characters
	for (PlayerCharacter* character : characters)
	{
		character->unregisterModels(staticModels, animatedModels);
		delete(character);
	}

	//clean up models
	for (Model* model : staticModels)
	{
		delete model;
	}
	for (AnimatedModel* model : animatedModels)
	{
		delete model;
	}

	//clean up pointers
	delete(camera);
	delete(staticShader);
	delete(animatedShader);
}