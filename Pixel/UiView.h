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
#include "FrameBuffer.h"

#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))

class UiView
{
public:
	UiView(GLFWwindow* window, Camera& camera, FrameBuffer& frameBuffer, float& width, float& height);
	~UiView();
	void InitializeNewFrame();
	void DrawUiFrame();
	void UiTransformSection(std::vector<ObjectStructure*>& model);
	void UiObjectSection(std::vector<ObjectStructure*>& model);
	void UiSceneSection(std::vector<ObjectStructure*>& model);
	void UiSliceSection();

	void SetOpenGLWindowMousePos(ImVec2 mousePos);
	ImVec2 GetOpenGLWindowMousePos();

private:
	ImVec2 OpenGLWindowMousePos;
	int m_selected = 0;
	Camera& m_camera;
	FrameBuffer& m_frameBuffer;
	float& m_width;
	float& m_height;
};

struct CustomConstraints
{
	static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->DesiredSize.x, data->DesiredSize.y); }
	static void Step(ImGuiSizeCallbackData* data) { float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step); }
};