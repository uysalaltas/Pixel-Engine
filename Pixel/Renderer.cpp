#include "Renderer.h"


Renderer::Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
	: m_vertices(vertices)
	, m_indices(indices)
{
	va.Bind();

	VertexBuffer vb(vertices);
	IndexBuffer ib(indices);

	va.AddBuffer(vb, 0, 3, sizeof(Vertex), (void*)0);
	va.AddBuffer(vb, 1, 3, sizeof(Vertex), (void*)(3 * sizeof(float)));
	va.AddBuffer(vb, 2, 3, sizeof(Vertex), (void*)(6 * sizeof(float)));
	va.AddBuffer(vb, 3, 2, sizeof(Vertex), (void*)(9 * sizeof(float)));

	va.Unbind();
	ib.Unbind();
	vb.Unbind();
}

//Renderer::Renderer(std::vector<Vertex>& vertices, std::vector<glm::uvec3>& indices)
//	: m_vertices(vertices)
//	, m_indices3(indices)
//{
//	va.Bind();
//
//	VertexBuffer vb(vertices);
//	IndexBuffer ib(glm::value_ptr(indices[0]), indices.size() * sizeof(glm::uvec3));
//	ib_size = indices.size() * 3;
//
//	va.AddBuffer(vb, 0, 3, sizeof(Vertex), (void*)0);
//	va.AddBuffer(vb, 1, 3, sizeof(Vertex), (void*)(3 * sizeof(float)));
//	va.AddBuffer(vb, 2, 3, sizeof(Vertex), (void*)(6 * sizeof(float)));
//	va.AddBuffer(vb, 3, 2, sizeof(Vertex), (void*)(9 * sizeof(float)));
//
//	va.Unbind();
//	ib.Unbind();
//	vb.Unbind();
//}

void Renderer::DrawLine(const Shader& shader) const
{
	va.Bind();
	shader.Bind();

	glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawTriangle(const Shader& shader) const
{
	va.Bind();
	shader.Bind();

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}