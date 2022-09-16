#include "Utils.h"

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		std::cout << "Shader Info Log: " << log << std::endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		std::cout << "Program Info Log: " << log << std::endl;
		free(log);
	}
}

bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		std::cout << "glError: " << glErr << std::endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

std::string readShaderSource(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram(const char* vp, const char* fp) {
	std::string vertShaderStr = readShaderSource(vp);
	std::string fragShaderStr = readShaderSource(fp);
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	// catch errors while compiling shaders
	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vShader);
	}
	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		printShaderLog(fShader);
	}
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	// catch errors while linking shaders
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		printProgramLog(vfProgram);
	}
	return vfProgram;
}

glm::mat4 viewTransform(glm::vec3 U, glm::vec3 V, glm::vec3 N, glm::vec3 cameraLoc) {
	glm::mat4 viewMatrix = {
		glm::vec4(U.x, V.x, N.x, 0),
		glm::vec4(U.y, V.y, N.y, 0),
		glm::vec4(U.z, V.z, N.z, 0),
		glm::vec4(0, 0, 0, 1)
	};
	viewMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLoc.x, -cameraLoc.y, -cameraLoc.z));
	return viewMatrix;
}

/*
	A common method to derive the view matrix is to compute a Look-at matrix given 
the position of the camera in world space (usually referred to as the “eye” position), 
an “up” vector (which is usually [0, 1, 0]T), and a target point to look at in world space.
*/
glm::mat4 lookAtCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up) {
	glm::vec3 forward = glm::normalize(eye - target);
	glm::vec3 side = glm::normalize(glm::cross(-forward, up));
	up = glm::normalize(glm::cross(side, -forward));

	glm::mat4 lookAtMatrix = {
		glm::vec4(side.x, up.x, forward.x, 0),
		glm::vec4(side.y, up.y, forward.y, 0),
		glm::vec4(side.z, up.z, forward.z, 0),
		glm::vec4(-glm::dot(side, eye), -glm::dot(up, eye), -glm::dot(forward, eye),  1)
	};

	return lookAtMatrix;
}