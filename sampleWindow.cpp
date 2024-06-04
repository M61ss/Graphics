#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WND_WIDTH 800
#define WND_HEIGHT 600

void fatalError(const char* message) 
{
	perror(message);
	exit(-1);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() 
{
	if (glfwInit() == GLFW_FALSE) 
		fatalError("Failed to init GLFW.\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, "LearneOpenGL", NULL, NULL);
	if (window == NULL)
		fatalError("Failed to init GLFW.\n");
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL();
	glViewport(0, 0, WND_WIDTH, WND_HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		//processInput(window);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}