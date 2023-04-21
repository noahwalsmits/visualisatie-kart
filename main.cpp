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

Camera* camera;
PlayerCharacter* playerCharacter;

Shader* staticShader;
std::vector<Model*> staticModels;
Shader* animatedShader;
std::vector<AnimatedModel*> animatedModels;


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
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	camera = new Camera();
	playerCharacter = new PlayerCharacter();
	playerCharacter->registerModels(staticModels, animatedModels);

	staticModels.push_back(new Model("assets/Cucumber/kart_YS_c.obj", glm::vec3(1.0f, 0.0f, 0.0f)));
	staticShader = new Shader("model.vs", "model.fs");

	animatedModels.push_back(new AnimatedModel("assets/animated_yoshi/yoshi.dae",
		{ "assets/animated_yoshi/yoshiLeft.dae", "assets/animated_yoshi/yoshiRight.dae" }, 
		glm::vec3(0.0f, 1.0f, 0.0f)));
	animatedShader = new Shader("model_animated.vs", "model.fs");

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT);
	}

	lastTime = glfwGetTime();

	std::cout << "completed init" << std::endl;
}


void display()
{
	glfwGetWindowSize(window, &screenSize.x, &screenSize.y);
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//prepare uniform data
	glm::mat4 projection = glm::perspective(glm::radians(80.0f), screenSize.x / (float)screenSize.y, 0.01f, 100.0f);
	glm::mat4 view = camera->getView();

	//apply to static shader and draw static models
	staticShader->use();
	staticShader->setMat4("projectionMatrix", projection);
	staticShader->setMat4("viewMatrix", view);
	for (Model* model : staticModels)
	{
		model->draw(*staticShader);
	}

	//apply to animated shader and draw animated models
	animatedShader->use();
	animatedShader->setMat4("projectionMatrix", projection);
	animatedShader->setMat4("viewMatrix", view);
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

	playerCharacter->update(elapsed, window);

	for (AnimatedModel* model : animatedModels)
	{
		model->update(elapsed);
	}

	//process keyboard input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//moving the camera
	float cameraSpeed = 3.0f * elapsed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 10.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->position.z -= cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->position.z += cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->position.x -= cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->position.x += cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera->position.y -= cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->position.y += cameraSpeed;
}


int main(int argc, char* argv[])
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1280, 1024, "Shaders week 1", NULL, NULL);

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

	playerCharacter->unregisterModels(staticModels, animatedModels);

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
	delete(playerCharacter);
	delete(staticShader);
	delete(animatedShader);
}