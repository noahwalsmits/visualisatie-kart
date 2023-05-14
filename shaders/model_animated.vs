#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in ivec4 boneIds; 
layout(location = 4) in vec4 weights;
	
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
	
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];
	
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition; //position in world space
	
void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >=MAX_BONES)
        {
            totalPosition = vec4(pos,1.0f);
            break;
        }
        vec4 localPosition = finalBoneMatrices[boneIds[i]] * vec4(pos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBoneMatrices[boneIds[i]]) * norm;
        totalNormal += localNormal * weights[i];
    }

    TexCoords = tex;
    Normal = mat3(transpose(inverse(modelMatrix))) * totalNormal;
    FragPosition = vec3(modelMatrix * vec4(pos, 1.0f));
	
    mat4 viewModel = viewMatrix * modelMatrix;
    gl_Position =  projectionMatrix * viewModel * totalPosition;
}