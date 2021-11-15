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

const float WIDTH = 720.0f;
const float HEIGHT = 720.0f;
float platfromSize = 200.0f;

bool firstMouseClick = true;
glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);
glm::vec2 currentMousePosClick = glm::vec2(0.0f, 0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void keyboard_callback(GLFWwindow* window);

glm::vec2 centerPlatformPoint = glm::vec2(platfromSize/2, platfromSize/2);
glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);

Camera camera(
	glm::vec3(0.0f, 0.0f, platfromSize * 3), 
	glm::vec3(centerPlatformPoint.x, centerPlatformPoint.y, 0.0f),
	upVector, WIDTH, HEIGHT
);

glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 modelPlatform = glm::mat4(1.0f);
glm::mat4 modelCube = glm::mat4(1.0f);
glm::mat4 modelShape = glm::mat4(1.0f);

int selectedObj = 0;
std::vector<glm::vec4*> objectPositions;

int main()
{
#pragma region OPENGL_SCENE_INIT
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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pixel Slicer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	std::vector<Texture> tempTexture;
	Renderer platform(vertices, indices, tempTexture);
	Renderer axis(verticesAxis, indicesAxis, tempTexture);

	glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


#pragma endregion

#pragma region IMGUI
	//const char* glsl_version = "#version 330";
	//ImGui::CreateContext();
	//ImGui::StyleColorsDark();
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init(glsl_version);
	//bool show_demo_window = true;
	//bool show_another_window = false;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	UiView uiView(window, WIDTH, HEIGHT);
#pragma endregion

	Shader shaderBasic("Basic.shader");
	//shaderBasic.Bind();

	Model cubeModel("Models/cube.stl", glm::vec3(1.0f, 0.2f, 1.0f));
	Model cubeModelLine("Models/cube.stl", glm::vec3(0.2f, 0.2f, 0.2f));

	Model simpleModel("Models/charmender.stl", glm::vec3(1.0f, 1.0f, 0.2f));
	Model simpleModelLine("Models/charmender.stl", glm::vec3(0.2f, 0.2f, 0.2f));

	objectPositions.push_back(&modelCube[3]);
	objectPositions.push_back(&modelShape[3]);

	FrameBuffer sceneBuffer(WIDTH, HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();
		//ImGuizmo::SetOrthographic(false);
		//ImGuizmo::BeginFrame();
		
		uiView.InitializeNewFrame();

		sceneBuffer.Bind();
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		{
			platform.Clear();
			axis.Clear();

			proj = glm::perspective(glm::radians(camera.GetFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, platfromSize * 20);
			view = camera.GetViewMatrix();
			
			glm::mat4 mvpAxisLine = proj * view * modelPlatform;
			shaderBasic.SetUniformMat4f("u_MVP", mvpAxisLine);
			axis.DrawLine(shaderBasic);
			
			glm::mat4 mvpPlatform = proj * view * modelPlatform;
			shaderBasic.SetUniformMat4f("u_MVP", mvpPlatform);
			platform.DrawLine(shaderBasic);
		}

		glm::mat4 mvpCube = proj * view * modelCube;
		shaderBasic.SetUniformMat4f("u_MVP", mvpCube);
		cubeModelLine.Draw(shaderBasic, GL_LINE);
		cubeModel.Draw(shaderBasic, GL_FILL);

		glm::mat4 mvpSimple = proj * view * modelShape;
		shaderBasic.SetUniformMat4f("u_MVP", mvpSimple);
		simpleModelLine.Draw(shaderBasic, GL_LINE);
		simpleModel.Draw(shaderBasic, GL_FILL);

		sceneBuffer.Unbind();
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			//ImGui::Begin("Pixel Engine");	
			//ImGui::SliderFloat3("Translation", &translations[selectedObj].x, 0.0f, 200.0f);
			//ImGui::SliderFloat3("Rotation", &rotations[selectedObj].x, 0, 360);
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//ImGui::End();
			//
			//ImGui::SetNextWindowSize(ImVec2(WIDTH*0.7f, HEIGHT*0.7f));
			//ImGui::Begin("Scene");
			//{
			//	ImGui::BeginChild("GameRender");
			//	unsigned int frameTexture = sceneBuffer.getFrameTexture();
			//	ImGui::Image((ImTextureID)frameTexture, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
			//	ImGuizmo::SetDrawlist();
			//	float windowWidth = (float)ImGui::GetWindowWidth();
			//	float windowHeight = (float)ImGui::GetWindowHeight();
			//	float windowPosX = ImGui::GetWindowPos().x;
			//	float windowPosY = ImGui::GetWindowPos().y;
			//	ImGuizmo::SetRect(windowPosX, windowPosY, windowWidth, windowHeight);
			//	ImGuizmo::Manipulate(
			//		glm::value_ptr(view),
			//		glm::value_ptr(proj),
			//		ImGuizmo::OPERATION::TRANSLATE,
			//		ImGuizmo::MODE::LOCAL,
			//		glm::value_ptr(modelCube)
			//	);
			//	ImGui::EndChild();
			//}
			//ImGui::End(); 
			
			if (selectedObj == -1)
			{
				selectedObj = 0;
			}
			uiView.DrawUiFrame(proj, view, modelCube, sceneBuffer.getFrameTexture());
		}

		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
				float winZ;
				glReadPixels(currentMousePosClick.x, HEIGHT - currentMousePosClick.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
				glm::vec3 objPt = glm::unProject(
					glm::vec3(currentMousePosClick.x, HEIGHT - currentMousePosClick.y, winZ),
					view,
					proj,
					glm::vec4(0, 0, WIDTH, HEIGHT)
				);

				size_t i = 0;
				float minDist = 1000;
				selectedObj = -1;

				std::cout << glm::to_string(objPt) << std::endl;

				for (i = 0; i < objectPositions.size(); i++) {
					glm::vec3 objectDistance = glm::vec3(
						objectPositions[i]->x,
						objectPositions[i]->y, 
						objectPositions[i]->z 
					);

					float dist = glm::distance(objectDistance, objPt);
					std::cout << dist << std::endl;

					if (dist < 200 && dist < minDist) {
						selectedObj = i;
						minDist = dist;
					}
				}

				std::cout << selectedObj << " " << i << std::endl;
			}
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
	else 
	{
		firstMouseClick = true;
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

//void ScreenPosToRay(
//	int mouseX, int mouseY,
//	glm::mat4 projMatrix,
//	glm::mat4 viewMatrix
//)
//{
//	float x = (2.0f * mouseX) / WIDTH - 1.0f;
//	float y = 1.0f - (2.0f * mouseY) / HEIGHT;
//	float z = 1.0f;
//	glm::vec3 ray_nds = glm::vec3(x, y, z);
//	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
//	glm::vec4 ray_eye = glm::inverse(projMatrix) * ray_clip;
//	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
//
//	glm::vec3 ray_wor = glm::vec3(
//		(glm::inverse(viewMatrix) * ray_eye).x,
//		(glm::inverse(viewMatrix) * ray_eye).y,
//		(glm::inverse(viewMatrix) * ray_eye).z
//	);
//
//	ray_wor = glm::normalize(ray_wor);
//}