#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;	
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTex;

out vec3 crntPos;
out vec3 color;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 u_MVP;

void main()
{
	crntPos = aPos;
	color = aColor;
	Normal = aNormal;
	texCoord = aTex;

	gl_Position = u_MVP * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 color;
in vec3 Normal;
in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
//uniform sampler2D texture_specular1;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	// ambient
	vec3 ambient = vec3(0.2f, 0.2f, 0.2f) * texture(texture_diffuse1, texCoord).rgb;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3((10.2f, 10.0f, 20.0f)) - crntPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(0.5f, 0.5f, 0.5f) * diff * texture(texture_diffuse1, texCoord).rgb;

	// specular
	//vec3 viewDir = normalize(camPos - crntPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0f);
	//vec3 specular = vec3(1.0f, 1.0f, 1.0f) * spec * texture(texture_specular1, texCoord).rgb;

	vec3 result = ambient + diffuse;
	FragColor = vec4(result, 1.0);
};