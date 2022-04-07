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
#include "Scene.h"


float platfromSize = 20.0f;
glm::vec2 centerPlatformPoint = glm::vec2(platfromSize/2, platfromSize/2);
glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);
glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);

std::vector<ObjectStructure*> objectStructures;
std::vector<Model*> objectModels;

int main()
{
#pragma region OPENGL_SCENE_INIT
	GLWindow gl_window;
	Camera camera(
		glm::vec3(100.0f, 0.0f, platfromSize * 3),
		glm::vec3(0.0f, 0.0f, 0.0f),
		upVector, &gl_window.WIDTH, &gl_window.HEIGHT

	);

	FrameBuffer sceneBuffer(gl_window.WIDTH, gl_window.HEIGHT);
	UiView uiView(gl_window.window, camera, sceneBuffer, gl_window.WIDTH, gl_window.HEIGHT);
	gl_window.InitCamera(&camera, &sceneBuffer);

	Shader shaderBasic("Basic.shader");
	Scene mainScene(objectStructures, objectModels);

	ObjectStructure cube;
	cube.path = "Models/eyeball.obj";
	cube.name = "cube";

	objectStructures.push_back(&cube);

	Intersection inter;
	Plane p;
	p.normal = glm::vec3(0, 0, 1);
	p.distance = 10;

#pragma endregion

	while (!glfwWindowShouldClose(gl_window.window))
	{
		uiView.InitializeNewFrame();
		sceneBuffer.Bind();
		gl_window.Bind();

		shaderBasic.SetUniform3f("lightPos", 0.0f, 0.0f, 0.0f);
		shaderBasic.SetUniform3f("lightColor", 0.8f, 0.8f, 0.8f);

		camera.UpdateProjMatrix();
		proj = camera.GetProjMatrix();
		view = camera.GetViewMatrix();

		mainScene.SceneUpdate(shaderBasic, camera);

		//std::vector<Point> outSegTips;
		//for (int i = 0; i < objectModels[0]->meshes[0]->m_triangles.size(); i++)
		//{
		//	inter.TrianglePlaneIntersection(objectModels[0]->meshes[0]->m_triangles[i], p, outSegTips);
		//}

		//sort(outSegTips.begin(), outSegTips.end(),
		//	[](const glm::vec3& lhs, const glm::vec3& rhs) {
		//		// Sort in ascending order by key x, y priority
		//		return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
		//	}
		//);
		//outSegTips.erase(unique(outSegTips.begin(), outSegTips.end()), outSegTips.end());

		//for (int i = 0; i < outSegTips.size(); i++)
		//{
		//	std::cout << "A: " << glm::to_string(outSegTips[i]) << std::endl;
		//}

		sceneBuffer.Unbind();
		gl_window.Unbind();

		uiView.UiObjectSection(objectStructures);
		uiView.UiTransformSection(objectStructures);
		uiView.UiSceneSection(objectStructures);
		uiView.UiSliceSection();
		uiView.DrawUiFrame();
		
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