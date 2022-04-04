#pragma once
#include <glad/glad.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Geometry3D.h"

class Renderer
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;
	std::vector<Triangle> m_triangles;

	Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures);
	~Renderer()
	{
		std::cout << "Renderer Deconstructor" << std::endl;
		delete vb;
		delete ib;
	}

	void Clear();
	void DrawLine(Shader& shader);
	void DrawTriangle(Shader& shader);

private:
	VertexArray va;
	VertexBuffer* vb;
	IndexBuffer* ib;
};