#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPosition; //position in world space

uniform sampler2D texture_diffuse1;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
    //texture
    if(texture(texture_diffuse1, TexCoords).a == 0.0f) //does not support translucency
    {
        discard;
    }
    FragColor = texture(texture_diffuse1, TexCoords);

    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 result = vec4(diffuse, 1.0);//
    //vec3 result = (ambient + diffuse) * objectColor;
    FragColor = FragColor * result;
}