#version 430

layout (location = 0) in vec3 position;

const float r_spd = 1.0f; // rotation speed
const float ds = 20.0f; // density of objects

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform float tf; // time factor for animation and placement of cubes

out vec4 varingColor;

mat4 buildRotateX(float rad); // declaration of matrix transformation utility functions
mat4 buildRotateY(float rad); // (GLSL requires functions to be declared prior to invocation)
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void) {
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	varingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

// UTILITIES
// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z)
{
	mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, y, z, 1.0);
	return trans;
}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad)
{
	mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
		0.0, cos(rad), -sin(rad), 0.0,
		0.0, sin(rad), cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0);
	return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad)
{
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(rad), 0.0, cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0);
	return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad)
{
	mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
		sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return zrot;
}