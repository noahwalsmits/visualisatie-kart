#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPosition; //fragment position in world space

uniform sampler2D texture_diffuse1;
uniform float ambientStrength;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
    //texture
    if(texture(texture_diffuse1, TexCoords).a == 0.0f) //does not support translucency
    {
        discard;
    }
    vec3 texture = vec3(texture(texture_diffuse1, TexCoords));

    //ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPosition);
    float diffuseStrength = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 result = (ambient + diffuse) * texture;
    FragColor = vec4(result, 1.0);
}