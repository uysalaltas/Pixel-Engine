#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "FrameBuffer.h"


class GLWindow
{
public:
	GLWindow();
	~GLWindow();
	
	float WIDTH = 1200.0f;
	float HEIGHT = 720.0f;
	GLFWwindow* window;

	void InitCamera(Camera* camera, FrameBuffer* frameBuffer);
	void Bind();
	void Unbind();

	static void window_size_callback_static(GLFWwindow* window, int width, int height);
	static void mouse_callback_static(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback_static(GLFWwindow* window, double xpos, double ypos);

	void window_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
private:

	glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);
	glm::vec2 currentMousePosClick = glm::vec2(0.0f, 0.0f);;
	bool firstMouseClick;
	Camera* m_camera;
	FrameBuffer* m_frameBuffer;
};
