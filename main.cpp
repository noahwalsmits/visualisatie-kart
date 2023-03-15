#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include "rendering/Shader.h"
#include "rendering/model.h"
#include "rendering/Camera.h"
#include "stb_image.h"

GLFWwindow* window;
glm::ivec2 screenSize;
double lastTime;

GLuint modelUniform;
GLuint viewUniform;
GLuint projectionUniform;

Camera* camera;

Model* eggCar; //TODO use references instead
Model* driver;
Shader* shader;

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

	eggCar = new Model("assets/egg1/egg1.obj", glm::vec3(0.0f, 0.0f, 0.0f));
	driver = new Model("assets/Yoshi/player/P_YS.obj", glm::vec3(0.0f, 1.0f, 0.0f));

	shader = new Shader("model.vs", "model.fs");
	shader->use();

	//create uniforms so we can set values to be used in the shader
	modelUniform = glGetUniformLocation(shader->ID, "modelMatrix");
	viewUniform = glGetUniformLocation(shader->ID, "viewMatrix");
	projectionUniform = glGetUniformLocation(shader->ID, "projectionMatrix");

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

	//projection
	glm::mat4 projection = glm::perspective(glm::radians(80.0f), screenSize.x / (float)screenSize.y, 0.01f, 100.0f);
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

	//view
	glm::mat4 view = camera->getView();
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));

	//model uniform is set in model draw call
	eggCar->draw(*shader, modelUniform);
	driver->draw(*shader, modelUniform);

	glfwSwapBuffers(window);
}


void update()
{
	double time = glfwGetTime();
	double elapsed = time - lastTime;
	lastTime = time;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//moving the camera
	float cameraSpeed = 3.0f * elapsed;
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

	//clean up pointers
	free(camera);
	free(eggCar);
	free(driver);
	free(shader);
}