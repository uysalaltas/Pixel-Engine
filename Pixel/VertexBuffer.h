#pragma once
#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer(GLfloat* data, GLsizeiptr size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;

};