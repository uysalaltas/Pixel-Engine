#pragma once
#include <glad/glad.h>


class IndexBuffer
{
public:
	IndexBuffer(GLuint* indices, GLsizeiptr count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; };
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};