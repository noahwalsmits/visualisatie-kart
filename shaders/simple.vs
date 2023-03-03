#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;

uniform mat4 modelViewProjectionMatrix;
uniform float time = 0;

out vec3 color;

void main()
{
	vec3 position = a_position;

//	position.y += sin(position.x * time);
	color = a_color;
	gl_Position = modelViewProjectionMatrix * vec4(position,1);
}