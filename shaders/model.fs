#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    if(texture(texture_diffuse1, TexCoords).a == 0.0f) //does not support translucency
    {
        discard;
    }
    FragColor = texture(texture_diffuse1, TexCoords);
}