#pragma once
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/ImGuizmo.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <vector>
#include <windows.h>

#include "Object.h"
#include "Camera.h"

#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))

class UiView
{
public:
	UiView(GLFWwindow* window, Camera* camera);
	~UiView();
	void InitializeNewFrame();
	void DrawUiFrame(glm::mat4& proj, glm::mat4& view, std::vector<ObjectStructure*>& model, unsigned int frameTexture);
	void SetOpenGLWindowMousePos(ImVec2 mousePos);
	ImVec2 GetOpenGLWindowMousePos();

private:
	ImVec2 OpenGLWindowMousePos;
	int selected = 0;
	bool _fullWindow = true;
	Camera* m_camera;
};

struct CustomConstraints
{
	static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->DesiredSize.x, data->DesiredSize.y); }
	static void Step(ImGuiSizeCallbackData* data) { float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step); }
};