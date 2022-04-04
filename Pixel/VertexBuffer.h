#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texUV;
};

class VertexBuffer
{
public:
	VertexBuffer(std::vector<Vertex>& vertices);
	~VertexBuffer();
	void BufferDataModification(std::vector<Vertex>& vertices);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;

};