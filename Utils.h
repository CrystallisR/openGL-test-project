#ifndef UTILS
#define UTILS

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>

void printShaderLog(GLuint shader);

void printProgramLog(int prog);

bool checkOpenGLError();

std::string readShaderSource(const char* filePath);

GLuint createShaderProgram(const char* vp, const char* fp);

glm::mat4 viewTransform(glm::vec3 U, glm::vec3 V, glm::vec3 N, glm::vec3 cameraLoc);

glm::mat4 lookAtCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up);

#endif