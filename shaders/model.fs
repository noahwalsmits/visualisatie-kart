#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPosition; //fragment position in world space

uniform sampler2D texture_diffuse1;
uniform float ambientStrength;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

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

    //specular lighting
    float specularStrength = 1.0;
    int shinyness = 64; //TODO make these configurable with uniforms
    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), shinyness);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * texture;
    FragColor = vec4(result, 1.0);
}