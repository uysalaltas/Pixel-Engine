#pragma once
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

class UiView
{
public:
	UiView(GLFWwindow* window, const float width, const float height);
	~UiView();
	void InitializeNewFrame();
	void DrawUiFrame(glm::mat4& proj, glm::mat4& view, glm::mat4& model, unsigned int frameTexture);
	void SetOpenGLWindowMousePos(ImVec2 mousePos);
	ImVec2 GetOpenGLWindowMousePos();

private:
	ImVec2 OpenGLWindowMousePos;
	float WIDTH;
	float HEIGHT;
};