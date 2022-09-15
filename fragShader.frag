#version 430

in vec4 varingColor;

out vec4 color;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf; // time factor for animation and placement of cubes

void main(void) { 
	color = varingColor;
}