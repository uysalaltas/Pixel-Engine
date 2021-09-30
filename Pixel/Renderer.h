#pragma once
#include <glad/glad.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer(std::vector<glm::vec3> vertices, std::vector<glm::uvec4> indices);
	Renderer(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> indices);

	void Clear();
	void DrawLine(const Shader& shader) const;
	void DrawTriangle(const Shader& shader) const;

	//void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	VertexArray va;
	float ib_size;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::uvec3> m_indices3;
	std::vector<glm::uvec4> m_indices4;
};