#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Renderer.h"
#include "Model.h"


float width = 720.0f;
float height = 720.0f;

float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//void keyboard_callback(GLFWwindow* window);
//glm::vec3 get_arcball_vector(int x, int y);

float platfromSize = 200.0f;
glm::vec2 centerPoint = glm::vec2(platfromSize/2, platfromSize/2);

glm::vec3 m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec2 m_lastMousePos = glm::vec2(0.0f, 0.0f);

Camera camera(
	glm::vec3(0.0f, 0.0f, platfromSize * 3), 
	glm::vec3(centerPoint.x, 0.0f, centerPoint.y),
	m_upVector, width, height
);

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

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	int slices = platfromSize / 10;

	std::vector<Vertex> verticesAxis = {
		Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(0.0f, 0.0f, 1.0f)},
		Vertex{glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f)}
	};

	std::vector<GLuint> indicesAxis = {
		0, 1, 
		2, 3, 
		4, 5
	};

	for (int j = 0; j <= slices; ++j) {
		for (int i = 0; i <= slices; ++i) {
			float x = (float)i * 10;
			float y = 0;
			float z = (float)j * 10;

			vertices.push_back(
				Vertex{ 
					glm::vec3(x, y, z), 
					glm::vec3(0.8f, 0.3f, 0.02f)
				});
		}
	}

	for (int j = 0; j < slices; ++j) {
		for (int i = 0; i < slices; ++i) {

			int row1 = j * (slices + 1);
			int row2 = (j + 1) * (slices + 1);

			indices.push_back(row1 + i);
			indices.push_back(row1 + i + 1);
			indices.push_back(row1 + i + 1);
			indices.push_back(row2 + i + 1);

			indices.push_back(row2 + i + 1);
			indices.push_back(row2 + i);
			indices.push_back(row2 + i);
			indices.push_back(row1 + i);
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
		
	Shader shaderBasic("Basic.shader");
	//shaderBasic.Bind();

	bool firstClick = true;
	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPos(window, (width / 2), (height / 2));
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

	std::vector<Texture> tempTexture;
	Renderer platform(vertices, indices, tempTexture);
	//modelPlatform = glm::rotate(modelPlatform, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	Renderer axis(verticesAxis, indicesAxis, tempTexture);

	glm::vec3 translation(0.0f, 0.0f, 0.0f);
	Model cubeModel("Models/boat.stl");
			
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		platform.Clear();
		axis.Clear();

		proj = glm::perspective(glm::radians(camera.GetFOV()), (float)width / (float)height, 0.1f, platfromSize * 20);
		view = camera.GetViewMatrix();

		glm::mat4 mvpAxisLine = proj * view * modelPlatform;
		shaderBasic.SetUniformMat4f("u_MVP", mvpAxisLine);
		axis.DrawLine(shaderBasic);

		glm::mat4 mvpPlatform = proj * view * modelPlatform;
		shaderBasic.SetUniformMat4f("u_MVP", mvpPlatform);
		platform.DrawLine(shaderBasic);

		modelCube = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvpCube = proj * view * modelCube;
		shaderBasic.SetUniformMat4f("u_MVP", mvpCube);
		cubeModel.Draw(shaderBasic);

		{
			ImGui::Begin("Pixel Engine");	
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 200.0f);
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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