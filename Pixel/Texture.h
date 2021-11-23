#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <stdexcept>

#include "Shader.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;
	std::string path;

	Texture(const char* filepath, const char* texType, GLuint unitType);
	~Texture();

	void texUnit(Shader& shader, const char* uniform, GLuint _unit);
	void Bind();
	void Unbind();
	void Delete();

private:


};
