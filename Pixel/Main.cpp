#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Renderer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

float width = 720.0f;
float height = 720.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void keyboard_callback(GLFWwindow* window);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//glm::vec3 get_arcball_vector(int x, int y);

glm::vec3 m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec2 m_lastMousePos = glm::vec2(0.0f, 0.0f);

Camera camera(glm::vec3(2.0f, 2.0f, 2.828f), glm::vec3(0.5f, 0.0f, 0.5f), m_upVector, width, height);

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 modelPlatform = glm::mat4(1.0f);
glm::mat4 modelCube = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);

int main()
{
	// -------------------------OPENGL FUNCTIONS-------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::vector<glm::vec3> verticesCube
	{
		glm::vec3(-0.1f, -0.1f,  0.1f),
		glm::vec3(0.1f, -0.1f,  0.1f),
		glm::vec3(-0.1f,  0.1f,  0.1f),
		glm::vec3(0.1f,  0.1f,  0.1f),
		glm::vec3(-0.1f, -0.1f, -0.1f),
		glm::vec3(0.1f, -0.1f, -0.1f),
		glm::vec3(-0.1f,  0.1f, -0.1f),
		glm::vec3(0.1f,  0.1f, -0.1f),
	};	

	std::vector<glm::uvec3> indicesCube
	{
		glm::vec3(2, 6, 7),
		glm::vec3(2, 3, 7),
		glm::vec3(0, 4, 5),
		glm::vec3(0, 1, 5),
		glm::vec3(0, 2, 6),
		glm::vec3(0, 4, 6),
		glm::vec3(1, 3, 7),
		glm::vec3(1, 5, 7),
		glm::vec3(0, 2, 3),
		glm::vec3(0, 1, 3),
		glm::vec3(4, 6, 7),
		glm::vec3(4, 5, 7)
	};

	std::vector<glm::vec3> vertices;
	std::vector<glm::uvec4> indices;
	int slices = 10;

	for (int j = 0; j <= slices; ++j) {
		for (int i = 0; i <= slices; ++i) {
			float x = (float)i / (float)slices;
			float y = 0;
			float z = (float)j / (float)slices;
			vertices.push_back(glm::vec3(x, y, z));
		}
	}

	for (int j = 0; j < slices; ++j) {
		for (int i = 0; i < slices; ++i) {

			int row1 = j * (slices + 1);
			int row2 = (j + 1) * (slices + 1);

			indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
			indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
		}
	}

	GLFWwindow* window = glfwCreateWindow(width, height, "Pixel Slicer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);
	// ------------------------------------------------------------------
		
	Shader shaderPlatform("Basic.shader");
	shaderPlatform.Bind();
	shaderPlatform.SetUniform4f("u_Color", 0.8f, 0.3f, 0.02f, 1.0f);

	//VertexArray va;
	//va.Bind();
	//VertexBuffer vb(vertices, sizeof(vertices));
	//IndexBuffer ib(indices, sizeof(indices));	
	//VertexBuffer vb(glm::value_ptr(vertices[0]), vertices.size() * sizeof(glm::vec3));
	//IndexBuffer ib(glm::value_ptr(indices[0]), indices.size() * sizeof(glm::uvec4));
	//va.AddBuffer(vb, 0);
	//shader.Unbind();
	//va.Unbind();
	//ib.Unbind();
	//vb.Unbind();

	bool firstClick = true;
	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPos(window, (width / 2), (height / 2));
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	// ------------------------- IMGUI -------------------------
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// ---------------------------------------------------------

	Renderer platform(vertices, indices);
	Renderer cube(verticesCube, indicesCube);

	//modelPlatform = glm::rotate(modelPlatform, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 translation(0.0f, 0.0f, 0.0f);
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		platform.Clear();
		cube.Clear();

		view = camera.GetViewMatrix();

		glm::mat4 mvpPlatform = proj * view * modelPlatform;
		shaderPlatform.SetUniform4f("u_Color", 0.8f, 0.3f, 0.02f, 1.0f);
		shaderPlatform.SetUniformMat4f("u_MVP", mvpPlatform);
		platform.DrawLine(shaderPlatform);

		modelCube = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvpCube = proj * view * modelCube;
		shaderPlatform.SetUniform4f("u_Color", 0.0f, 0.8f, 0.2f, 1.0f);
		shaderPlatform.SetUniformMat4f("u_MVP", mvpCube);
		cube.DrawTriangle(shaderPlatform);

		{
			ImGui::Begin("Pixel Engine");	
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 1.0f);
			ImGui::SameLine();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (firstMouse)
		{
			m_lastMousePos.x = xpos;
			m_lastMousePos.y = ypos;
			firstMouse = false;

		}
		camera.ArcBallCamera((m_lastMousePos.x - xpos), (m_lastMousePos.y - ypos));
		m_lastMousePos.x = xpos;
		m_lastMousePos.y = ypos;
	}
	else 
	{
		firstMouse = true;
	}
}

//void keyboard_callback(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		camera.ProcessKeyboard(FORWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		camera.ProcessKeyboard(BACKWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		camera.ProcessKeyboard(LEFT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		camera.ProcessKeyboard(RIGHT, deltaTime);
//}

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(yoffset);
//}

//glm::vec3 get_arcball_vector(int x, int y) {
//	glm::vec3 P = glm::vec3(1.0 * x / width * 2 - 1.0, 1.0 * y / height * 2 - 1.0, 0);
//	P.y = -P.y;
//	float OP_squared = P.x * P.x + P.y * P.y;
//	if (OP_squared <= 1 * 1)
//		P.z = sqrt(1 * 1 - OP_squared);
//	else
//		P = glm::normalize(P);
//	return P;
//}