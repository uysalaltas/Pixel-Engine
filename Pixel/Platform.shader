#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 crntPos;
out vec3 color;

uniform mat4 u_MVP;

void main()
{
	crntPos = aPos;
	color = aColor;

	gl_Position = u_MVP * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 color;

void main()
{
	vec4 texColor = vec4(color, 1.0);
	FragColor = texColor;
};