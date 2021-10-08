#pragma once
#include <glad/glad.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

struct MeshData
{
	std::vector<Vertex> meshVertices;
	std::vector<GLuint> meshIndices;
};

class Renderer
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;

	Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

	void Clear();
	void DrawLine(const Shader& shader) const;
	void DrawTriangle(const Shader& shader) const;

	//void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	//void Setup(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

private:
	VertexArray va;
};