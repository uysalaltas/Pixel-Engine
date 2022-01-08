#include <iostream>

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
#include "GLWindow.h"


float platfromSize = 200.0f;
glm::vec2 centerPlatformPoint = glm::vec2(platfromSize/2, platfromSize/2);
glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);

glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 modelPlatform = glm::mat4(1.0f);

std::vector<ObjectStructure*> objectStructures;
std::vector<Model*> objectModels;

int main()
{
#pragma region OPENGL_SCENE_INIT
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
	
	GLWindow gl_window;

	Camera camera(
		glm::vec3(100.0f, 0.0f, platfromSize * 3),
		glm::vec3(centerPlatformPoint.x, centerPlatformPoint.y, 0.0f),
		upVector, &gl_window.WIDTH, &gl_window.HEIGHT

	);

	std::vector<Texture> tempTexture;
	Renderer platform(vertices, indices, tempTexture);
	Renderer axis(verticesAxis, indicesAxis, tempTexture);
#pragma endregion

	FrameBuffer sceneBuffer(gl_window.WIDTH, gl_window.HEIGHT);
	UiView uiView(gl_window.window, &camera, &sceneBuffer, &gl_window.WIDTH, &gl_window.HEIGHT);
	gl_window.InitCamera(&camera, &sceneBuffer);

	Shader shaderBasic("Basic.shader");
	//shaderBasic.Bind();

	ObjectStructure cube;
	cube.path = "Models/charmender.stl";
	cube.name = "charmender.stl";

	objectStructures.push_back(&cube);

	while (!glfwWindowShouldClose(gl_window.window))
	{
		uiView.InitializeNewFrame();
		sceneBuffer.Bind();
		gl_window.Bind();

		{
			shaderBasic.SetUniform3f("lightPos", 0.0f, 0.0f, 0.0f);
			shaderBasic.SetUniform3f("lightColor", 0.8f, 0.8f, 0.8f);

			platform.Clear();
			axis.Clear();

			camera.UpdateProjMatrix();
			proj = camera.GetProjMatrix();
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
		gl_window.Unbind();
		uiView.DrawUiFrame(proj, view, objectStructures);
		
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(gl_window.window);
		glfwPollEvents();
	}

	for (int i = 0; i < objectModels.size(); i++)
	{
		delete objectModels[i];
	}

	return 0;
}