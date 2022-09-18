#include "solar.h"

using namespace std;

void solar::setupView(void) {
	// view transform: camera location and angle settings
	cameraLoc = glm::vec3(0.0f, 0.0f, 50.0f);
	U = glm::vec3(1.0f, 0.0f, 0.0f);
	V = glm::vec3(0.0f, 1.0f, 0.0f);
	N = glm::vec3(0.0f, 0.0f, 1.0f);

	// Look at camera settings
	eye = glm::vec3(0.0f, 0.0f, 40.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f); // x, y, z order
}

void solar::setupVertices(void) {
	std::vector<int> ind = astrosphere.getIndices();
	std::vector<glm::vec3> vert = astrosphere.getVertices();
	std::vector<glm::vec2> tex = astrosphere.getTexCoords();
	std::vector<glm::vec3> norm = astrosphere.getNormals();

	std::vector<float> pvalues; // vertex positions
	std::vector<float> tvalues; // texture coordinates
	std::vector<float> nvalues; // normal vectors
	int numIndices = astrosphere.getNumIndices();

	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);

		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);

		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	glGenVertexArrays(numSolarVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numSolarVBOs, vbo);

	// put the vertices into buffer #0
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	// put the texture coordinates into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	// put the normals into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

}

void solar::setpMat(float aspect) {
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
}

void solar::init(GLFWwindow* window) {
	renderingProgram = createShaderProgram("vertShader.vert", "fragShader.frag");
	setupView();
	setupVertices();
	// build perspective matrix
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	setpMat((float)width / (float)height);
	// load textures
	sunTexture = loadTexture("res/sun.jpg");
	moonTexture = loadTexture("res/moon.jpg");
	earthTexture = loadTexture("res/earth.jpg");
	JupiterTexture = loadTexture("res/Jupiter.jpg");
}

void solar::display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glEnable(GL_CULL_FACE);

	// bind texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// push view matrix onto the stack
	// vMat = viewTransform(U, V, N, cameraLoc);
	/*target = glm::vec3(sin((float)currentTime) * 20.0, 0.0f, cos((float)currentTime) * 20.0) +
		glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0); */
	vMat = lookAtCamera(eye, target, up);
	mvStack.push(vMat);

	// ---------------------- pyramid == sun --------------------------------------------
	// set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);

	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f)); // sun rotation
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(ssz, ssz, ssz)); // enlarge sun size

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, astrosphere.getNumIndices()); // finish drawing sun
	mvStack.pop(); // remove the sun’s axial rotation from the stack

	//----------------------- cube == planet ---------------------------------------------
	// set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);

	mvStack.push(mvStack.top());
	mvStack.top() *= // planet rotation around sun
		glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 20.0, 0.0f, cos((float)currentTime) * 20.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f)); // planet rotation
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(psz1, psz1, psz1)); // shrink planet size

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, astrosphere.getNumIndices()); // draw the planet
	mvStack.pop(); // remove the planet’s axial rotation from the stack

	//----------------------- smaller cube == moon -----------------------------------
	// set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonTexture);

	mvStack.push(mvStack.top());
	mvStack.top() *=
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f)); // moon rotation
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(msz, msz, msz)); // shrink moon size

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, astrosphere.getNumIndices()); // draw the moon
	mvStack.pop();

	//----------------------- cube == planet 2 ---------------------------------------------
	// set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, JupiterTexture);
	mvStack.pop(); // pop planet 1
	mvStack.push(mvStack.top());
	mvStack.top() *= // planet rotation around sun
		glm::translate(glm::mat4(1.0f), glm::vec3(sin(-(float)currentTime) * 20.0,
			sin(-(float)currentTime) * 10.0, cos(-(float)currentTime) * 20.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), 10 * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f)); // planet rotation
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(psz2, psz2, psz2)); // shrink planet size

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, astrosphere.getNumIndices()); // draw the planet
	mvStack.pop(); // remove planet2 axial rotation

	// remove planet position, sun position and view matrices from stack
	mvStack.pop(); mvStack.pop(); mvStack.pop();
}