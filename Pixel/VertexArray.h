#pragma once
#include "VertexBuffer.h"
#include <vector>
#include <glad/glad.h>


class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, GLuint layout, GLuint numComponents, GLsizeiptr stride, void* offset);
	void Bind() const;
	void Unbind() const;

private:
};