#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>

Renderer::Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures)
	: m_vertices(vertices)
	, m_indices(indices)
	, m_textures(textures)
{
	va.Bind();

	for (int i = 0; i < m_indices.size(); i++)
	{
		Point a = m_vertices[m_indices[i]].position;
		Vertex& aV = m_vertices[m_indices[i]];
		i++;
		Point b = m_vertices[m_indices[i]].position;
		Vertex& bV = m_vertices[m_indices[i]];
		i++;
		Point c = m_vertices[m_indices[i]].position;
		Vertex& cV = m_vertices[m_indices[i]];
		Triangle tri = { a, b, c, aV, bV, cV };
		m_triangles.push_back(tri);
	}

	vb = new VertexBuffer(m_vertices);
	ib = new IndexBuffer(m_indices);

	va.AddBuffer(*vb, 0, 3, sizeof(Vertex), (void*)0);
	va.AddBuffer(*vb, 1, 3, sizeof(Vertex), (void*)(3 * sizeof(float)));
	va.AddBuffer(*vb, 2, 3, sizeof(Vertex), (void*)(6 * sizeof(float)));
	va.AddBuffer(*vb, 3, 2, sizeof(Vertex), (void*)(9 * sizeof(float)));

	va.Unbind();
	vb->Unbind();
	ib->Unbind();

	std::cout << "Renderer Constructed" << std::endl;
}	

void Renderer::DrawLine(Shader& shader)
{
	va.Bind();
	shader.Bind();

	glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawTriangle(Shader& shader)
{
	shader.Bind();
	va.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 1;
	unsigned int numSpecular = 1;

	//std::cout << "SIZE TEXT " << m_textures.size() << std::endl;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		//std::cout << "PATH " << m_textures[i].path << std::endl;

		std::string num;
		std::string type = m_textures[i].type;

		if (type == "texture_diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "texture_specular")
		{
			num = std::to_string(numSpecular++);
		}

		m_textures[i].Bind();
		m_textures[i].texUnit(shader, "texture_diffuse0", i);
	}

	vb->BufferDataModification(m_vertices);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}