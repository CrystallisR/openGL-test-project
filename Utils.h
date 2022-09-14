#ifndef UTILS
#define UTILS

#include <GL\glew.h>
#include <GLFW\glfw3.h>
//#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <fstream>

void printShaderLog(GLuint shader);

void printProgramLog(int prog);

bool checkOpenGLError();

std::string readShaderSource(const char* filePath);

GLuint createShaderProgram(const char* vp, const char* fp);

#endif