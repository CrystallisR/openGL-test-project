#version 430

layout (location = 0) in vec3 position;
layout (location=1) in vec2 texCoord;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec2 tc; // texture coordinate output to rasterizer for interpolation
layout (binding=0) uniform sampler2D samp; // not used in vertex shader

void main(void) {
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	tc = texCoord;
}