#pragma once
#include <glad/glad.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	Renderer(std::vector<Vertex>& vertices, std::vector<glm::uvec3>& indices);

	void Clear();
	void DrawLine(const Shader& shader) const;
	void DrawTriangle(const Shader& shader) const;

	//void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	VertexArray va;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<glm::uvec4> m_indices4;
};