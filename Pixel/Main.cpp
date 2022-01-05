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
#include <imgui/ImGuizmo.h>

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Renderer.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "UiView.h"
#include "Object.h"
#include "Window.h"


float platfromSize = 200.0f;
bool firstMouseClick = true;
glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);
glm::vec2 currentMousePosClick = glm::vec2(0.0f, 0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec2 centerPlatformPoint = glm::vec2(platfromSize/2, platfromSize/2);
glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);

Camera camera(
	glm::vec3(100.0f, 0.0f, platfromSize * 3), 
	glm::vec3(centerPlatformPoint.x, centerPlatformPoint.y, 0.0f),
	upVector, Window::Get().WIDTH, Window::Get().HEIGHT
);

glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 modelPlatform = glm::mat4(1.0f);

std::vector<ObjectStructure*> objectStructures;
std::vector<Model*> objectModels;

int main()
{
#pragma region OPENGL_SCENE_INIT
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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
			float y = (float)j * 10;
			float z = 0;

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
	GLFWwindow* window = glfwCreateWindow(Window::Get().WIDTH, Window::Get().HEIGHT, "Pixel Slicer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, Window::Get().WIDTH, Window::Get().HEIGHT);
	glEnable(GL_DEPTH_TEST);

	std::vector<Texture> tempTexture;
	Renderer platform(vertices, indices, tempTexture);
	Renderer axis(verticesAxis, indicesAxis, tempTexture);

	glfwSetCursorPos(window, (Window::Get().WIDTH / 2), (Window::Get().HEIGHT / 2));
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion

#pragma region IMGUI
	UiView uiView(window);
#pragma endregion

	Shader shaderBasic("Basic.shader");
	//shaderBasic.Bind();

	ObjectStructure cube;
	cube.path = "Models/charmender.stl";
	cube.name = "charmender.stl";

	objectStructures.push_back(&cube);

	FrameBuffer sceneBuffer(Window::Get().WIDTH, Window::Get().HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		uiView.InitializeNewFrame();

		sceneBuffer.Bind();
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			shaderBasic.SetUniform3f("lightPos", 0.0f, 0.0f, 0.0f);
			shaderBasic.SetUniform3f("lightColor", 0.8f, 0.8f, 0.8f);

			platform.Clear();
			axis.Clear();

			proj = glm::perspective(glm::radians(camera.GetFOV()), (float)Window::Get().WIDTH / (float)Window::Get().HEIGHT, 0.1f, platfromSize * 20);
			view = camera.GetViewMatrix();
			
			glm::mat4 mvpAxisLine = proj * view * modelPlatform;
			shaderBasic.SetUniformMat4f("u_MVP", mvpAxisLine);
			axis.DrawLine(shaderBasic);
			
			glm::mat4 mvpPlatform = proj * view * modelPlatform;
			shaderBasic.SetUniformMat4f("u_MVP", mvpPlatform);
			platform.DrawLine(shaderBasic);
		}

		for (int i = 0; i < objectStructures.size(); i++)
		{
			if (!objectStructures[i]->modelDefined) {
				Model* model = new Model(objectStructures[i]);
				objectModels.push_back(model);
				objectStructures[i]->modelDefined = true;
			}
		}

		for (int i = 0; i < objectModels.size(); i++)
		{
			glm::mat4 mvp = proj * view * objectStructures[i]->objModel;
			shaderBasic.SetUniformMat4f("u_MVP", mvp);
			objectModels[i]->Draw(shaderBasic, GL_FILL);
		}

		sceneBuffer.Unbind();
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{	
			uiView.DrawUiFrame(proj, view, objectStructures, sceneBuffer.getFrameTexture());
			ImGui::Begin("Transform");
			ImGui::End();

		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (int i = 0; i < objectModels.size(); i++)
	{
		delete objectModels[i];
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	currentMousePosClick.x = xpos;
	currentMousePosClick.y = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (firstMouseClick)
		{
			lastMousePosRightClick.x = xpos;
			lastMousePosRightClick.y = ypos;
			firstMouseClick = false;
		}

		camera.ArcBallCamera((lastMousePosRightClick.x - xpos), (lastMousePosRightClick.y - ypos));
		lastMousePosRightClick.x = xpos;
		lastMousePosRightClick.y = ypos;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (firstMouseClick)
		{
			lastMousePosRightClick.x = xpos;
			lastMousePosRightClick.y = ypos;
			firstMouseClick = false;
		}
		//std::cout << "LAST MOUSE POS: " << glm::to_string(camera.GetLookAt()) << std::endl;
		camera.PanCamera(lastMousePosRightClick - currentMousePosClick);
		lastMousePosRightClick.x = xpos;
		lastMousePosRightClick.y = ypos;
	}
	else 
	{
		firstMouseClick = true;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}