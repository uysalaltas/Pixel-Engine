#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;	
layout(location = 2) in vec3 aNormal;

out VS_OUT
{
	vec3 CrntPos;
	vec3 Color;
	vec3 Normal;
} vs_out;

uniform mat4 u_MVP;
uniform mat4 model;

void main()
{
	vs_out.CrntPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Color = aColor;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;

	gl_Position = u_MVP * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 CrntPos;
	vec3 Color;
	vec3 Normal;
} fs_in;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	// ambient
	float ambientStrength = 0.8f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse 
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.CrntPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(camPos - fs_in.CrntPos);
	vec3 reflectDir = reflect(-lightDir, fs_in.Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * fs_in.Color;

	vec3 result = (ambient + diffuse + specular) * fs_in.Color;
	FragColor = vec4(result, 1.0);
};