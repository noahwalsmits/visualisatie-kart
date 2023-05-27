#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 aBoneIds;
layout(location = 4) in vec4 aWeights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBoneMatrices[MAX_BONES];
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition; //fragment position in world space

void main()
{
    //apply bone transformations to position and normal
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(aBoneIds[i] == -1)
            continue;
        if(aBoneIds[i] >=MAX_BONES)
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBoneMatrices[aBoneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * aWeights[i];
        vec3 localNormal = mat3(finalBoneMatrices[aBoneIds[i]]) * aNormal;
        totalNormal += localNormal * aWeights[i];
    }
    
    TexCoords = aTexCoords;
    Normal = mat3(transpose(inverse(modelMatrix))) * totalNormal;
    FragPosition = vec3(modelMatrix * vec4(aPos, 1.0f));
	
    mat4 viewModel = viewMatrix * modelMatrix;
    gl_Position =  projectionMatrix * viewModel * totalPosition;
}