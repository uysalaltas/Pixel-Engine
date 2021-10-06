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
uniform mat4 model;

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

out vec3 FragColor;

in vec3 crntPos;
in vec3 color;
in vec3 Normal;
in vec2 texCoord;

uniform vec4 u_Color;

void main()
{
   FragColor = color;
};