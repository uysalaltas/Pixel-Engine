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
		glm::vec3(500.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		upVector, &gl_window.WIDTH, &gl_window.HEIGHT

	);

	FrameBuffer sceneBuffer(gl_window.WIDTH, gl_window.HEIGHT);
	UiView uiView(gl_window.window, camera, sceneBuffer, gl_window.WIDTH, gl_window.HEIGHT);
	gl_window.InitCamera(&camera, &sceneBuffer);

	Shader shaderBasic("Basic.shader");
	Scene mainScene(objectStructures, objectModels);

	ObjectStructure* eye = new ObjectStructure();
	eye->path = "Models/cube.stl";
	eye->name = "cube";
	objectStructures.push_back(eye);

	Intersection inter;
	Plane p;
	p.normal = glm::vec3(0, 0, 1);
	p.distance = 0.0f;
	float layerHeight = 0.2f;

	std::vector<std::vector<TriangleIntersect>> results;
#pragma endregion

	bool tryNow = false;

	while (!glfwWindowShouldClose(gl_window.window))
	{
		uiView.InitializeNewFrame();
		sceneBuffer.Bind();
		gl_window.Bind();

		camera.UpdateProjMatrix();
		proj = camera.GetProjMatrix();
		view = camera.GetViewMatrix();

		mainScene.SceneUpdate(shaderBasic, camera);

		sceneBuffer.Unbind();
		gl_window.Unbind();

		uiView.UiObjectSection(objectStructures);
		uiView.UiTransformSection(objectStructures);
		uiView.UiSceneSection(objectStructures);
		//uiView.UiSliceSection(objectModels);

		ImGui::Begin("Slice");
		if (ImGui::Button("Slice"))
		{
			std::cout << "Button Clicked " << " " << std::endl;
		
			for (int dist = 0; dist < 100; dist++)
			{
				std::vector<TriangleIntersect> result;
				for (int i = 0; i < objectModels[0]->meshes.size(); i++)
				{
					for (int j = 0; j < objectModels[0]->meshes[i]->m_triangles.size(); j++)
					{
						//std::cout << "Next Mesh" << std::endl;
						inter.TrianglePlaneIntersection(objectModels[0]->meshes[i]->m_triangles[j], p, result);
					}
				}
				p.distance = p.distance + layerHeight;
				results.push_back(result);
			}

			for (int ind = 0; ind < results.size(); ind++)
			{
				std::cout << ";layer " << ind << std::endl;

				std::vector<Point> resultReorder;

				Point tempPoint = results[ind][0].points[1];
				Point firstTempPoint = results[ind][0].points[1];
				resultReorder.push_back(results[ind][0].points[0]);
				bool loopBreak = false;

				int loopIndice = 1;
				int offset = 100;

				for (int i = 0; i < results[ind].size(); i++)
				{
					std::cout << "DBG1: " << glm::to_string(results[ind][i].points[0]) << std::endl;
					std::cout << "DBG2: " << glm::to_string(results[ind][i].points[1]) << std::endl;
				}

				while (loopIndice != results[ind].size())
				{
					//std::cout << loopIndice << std::endl;
					for (int i = 0; i < results[ind].size(); i++)
					{
						if (results[ind][i].points[0] != results[ind][i].points[1])
						{
							if (tempPoint == results[ind][i].points[0])
							{
								resultReorder.push_back(tempPoint);
								loopIndice++;

								tempPoint = results[ind][i].points[1];
								break;
							}
						}
					}

					if (firstTempPoint == tempPoint)
					{
						loopBreak = true;
						break;
					}
				}

				if (!loopBreak)
				{
					resultReorder.push_back(results[ind][0].points[0]);
				}

				for (int i = 0; i < resultReorder.size(); i++)
				{
					std::cout << "G1 " << "X" << resultReorder[i].x + offset << " Y" << resultReorder[i].y + offset << std::endl;
				}
				std::cout << "G0 F300 " << "X" << resultReorder[0].x + offset << " Y" << resultReorder[0].y + offset << " Z" << resultReorder[0].z << std::endl;
			}
		}
		ImGui::End();

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