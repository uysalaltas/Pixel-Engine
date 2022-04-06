#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;	
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTex;
layout(location = 4) in vec2 aTangent;
layout(location = 5) in vec2 aBitangent;
layout(location = 6) in vec2 aBoneid;
layout(location = 7) in vec2 aWeight;

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
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambientStrength = 0.8f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec4 texColor = vec4((ambient + diffuse), 1.0) * vec4(color, 1.0);
	FragColor = texture(texture_diffuse1, texCoord) + texColor;
};