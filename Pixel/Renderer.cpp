#include "Renderer.h"


Renderer::Renderer(std::vector<glm::vec3> vertices, std::vector<glm::uvec4> indices)
	: m_vertices(vertices)
	, m_indices4(indices)
{
	va.Bind();

	VertexBuffer vb(glm::value_ptr(vertices[0]), vertices.size() * sizeof(glm::vec3));
	IndexBuffer ib(glm::value_ptr(indices[0]), indices.size() * sizeof(glm::uvec4));
	ib_size = ib.GetCount() / 4;

	va.AddBuffer(vb, 0);
	va.Unbind();
	ib.Unbind();
	vb.Unbind();
}

Renderer::Renderer(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> indices)
	: m_vertices(vertices)
	, m_indices3(indices)
{
	va.Bind();

	VertexBuffer vb(glm::value_ptr(vertices[0]), vertices.size() * sizeof(glm::vec3));
	IndexBuffer ib(glm::value_ptr(indices[0]), indices.size() * sizeof(glm::uvec3));
	ib_size = indices.size() * 3;

	va.AddBuffer(vb, 0);
	va.Unbind();
	ib.Unbind();
	vb.Unbind();
}

void Renderer::DrawLine(const Shader& shader) const
{
	va.Bind();
	shader.Bind();

	glDrawElements(GL_LINES, ib_size, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawTriangle(const Shader& shader) const
{
	va.Bind();
	shader.Bind();

	glDrawElements(GL_TRIANGLES, ib_size, GL_UNSIGNED_INT, 0);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}