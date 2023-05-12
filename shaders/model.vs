#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition; //position in world space

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    TexCoords = aTexCoords;
    Normal = aNormal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0); //the order of multiplication is important here
    FragPosition = vec3(modelMatrix * vec4(aPos, 1.0));
}