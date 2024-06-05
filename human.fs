#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform DirLight light;
uniform sampler2D texture_diffuse1;

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    if(diff > 0.0)
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    }

    vec3 ambient = light.ambient * vec3( texture( texture_diffuse1, TexCoords ));
    vec3 diffuse = light.diffuse * diff * vec3(texture( texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture( texture_diffuse1, TexCoords));

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
