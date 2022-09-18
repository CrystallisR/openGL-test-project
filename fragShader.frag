#version 430

in vec2 tc; // texture coordinates

out vec4 color;

uniform mat4 proj_matrix;
uniform float tf; // time factor for animation and placement of cubes

layout(binding = 0) uniform sampler2D samp;

void main(void) { 
	color = texture(samp, tc);
}