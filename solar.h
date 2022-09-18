#pragma once
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
#include "Sphere.h"

#define numSolarVAOs 1
#define numSolarVBOs 3

class solar {
private:
	// object
	Sphere astrosphere{48};
	GLuint renderingProgram{};
	// texture
	GLuint sunTexture{};
	GLuint moonTexture{};
	GLuint earthTexture{};
	GLuint JupiterTexture{};
	// structure
	GLuint vao[numSolarVAOs]{};
	GLuint vbo[numSolarVBOs]{};
	std::stack<glm::mat4> mvStack{};

	// parameters
	float ssz{ 6.0f }, psz1{ 1.5f }, psz2{ 3.0f }, msz{ 0.6f }; // size of spheres
	// camera settings
	glm::vec3 cameraLoc{}, U{}, V{}, N{}, eye{}, target{}, up{};
	GLuint mvLoc{}, projLoc{};
	glm::mat4 pMat{}, vMat{}, mMat{};

	void setupView(void);
	void setupVertices(void);	

public:
	solar() {};
	~solar() {};
	void setpMat(float aspect);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
};

