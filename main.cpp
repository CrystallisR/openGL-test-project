#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Utils.h"
#include "solar.h"

using namespace std;

solar solar_sys;

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	float aspect = (float)newWidth / (float)newHeight; // new width&height provided by the callback
	glViewport(0, 0, newWidth, newHeight); // sets screen region associated with framebufer
	solar_sys.setpMat(aspect); // using specified perspective matrix
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(1080, 720, "A Simple Solar System", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_reshape_callback);

	solar_sys.init(window);
	while (!glfwWindowShouldClose(window)) {
		solar_sys.display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}