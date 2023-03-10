#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include "rendering/Shader.h"
#include "stb_image.h"
#include "rendering/model.h"

GLFWwindow* window;
glm::ivec2 screenSize;
GLuint modelViewUniform;
GLuint timeUniform;
double lastTime;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 0.0f;

//RenderModel renderModel("egg1/egg1.obj");//
Model* eggCar; //TODO use references instead
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

	eggCar = new Model("assets/egg1/egg1.obj"); //TODO clean up memory

	shader = new Shader("model.vs", "model.fs"); //TODO clean up memory
	shader->use();

	modelViewUniform = glGetUniformLocation(shader->ID, "modelViewProjectionMatrix");
	timeUniform = glGetUniformLocation(shader->ID, "time");

	//glEnableVertexAttribArray(0);//
	//glEnableVertexAttribArray(1);//
	//glEnableVertexAttribArray(2);//

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT);
	}

	lastTime = glfwGetTime();
}


void display()
{
	glfwGetWindowSize(window, &screenSize.x, &screenSize.y);
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mvp = glm::perspective(glm::radians(80.0f), screenSize.x / (float)screenSize.y, 0.01f, 100.0f);
	mvp = glm::translate(mvp, glm::vec3(cameraX, cameraY, cameraZ)); //move camera
	float cameraRotationX = sin(glfwGetTime()) * 2.0f;
	float cameraRotationZ = cos(glfwGetTime()) * 2.0f;
	mvp *= glm::lookAt(glm::vec3(cameraRotationX, 0.0f, cameraRotationZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate camera round point
	glUniformMatrix4fv(modelViewUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(timeUniform, (float)lastTime);

	eggCar->draw(*shader);
	//renderModel.draw();//

	glfwSwapBuffers(window);
}


void update()
{
	double time = glfwGetTime();
	double elapsed = time - lastTime;
	lastTime = time;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraZ += 0.05f;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraZ -= 0.05f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraX += 0.05f;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraX -= 0.05f;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cameraY += 0.05f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraY -= 0.05f;
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
}