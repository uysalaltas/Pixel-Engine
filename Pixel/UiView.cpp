#include "UiView.h"

UiView::UiView(GLFWwindow* window, const float width, const float height)
{
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	WIDTH = width;
	HEIGHT = width;
}

UiView::~UiView()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UiView::InitializeNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
}

void UiView::DrawUiFrame(glm::mat4& proj, glm::mat4& view, glm::mat4& model, unsigned int frameTexture)
{
	ImGui::Begin("Objects");
	static bool selection = false;
	ImGui::Selectable("Object ", &selection);
	ImGui::End();

	ImGui::Begin("Transform");
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(model, scale, rotation, translation, skew, perspective);
	rotation = glm::conjugate(rotation);
	ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 200.0f);
	ImGui::SliderFloat3("Rotation", &rotation.x, 0, 360);
	model = glm::translate(glm::mat4(1.0f), translation);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(WIDTH * 0.7f, HEIGHT * 0.7f));
	ImGui::Begin("Scene", 0, ImGuiWindowFlags_NoMove);
	{
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)frameTexture, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		float windowPosX = ImGui::GetWindowPos().x;
		float windowPosY = ImGui::GetWindowPos().y;
		ImGuizmo::SetRect(windowPosX, windowPosY, windowWidth, windowHeight);
		ImGuizmo::Manipulate(
			glm::value_ptr(view),
			glm::value_ptr(proj),
			ImGuizmo::OPERATION::TRANSLATE,
			ImGuizmo::MODE::LOCAL,
			glm::value_ptr(model)
		);
		if (ImGuizmo::IsUsing)
		{
			translation = glm::vec3(model[3]);
		}
		ImVec2 screen_pos = ImVec2( (ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x) , (ImGui::GetCursorScreenPos().y - ImGui::GetMousePos().y));
		SetOpenGLWindowMousePos(screen_pos);
		ImGui::EndChild();
	}
	ImGui::End();


}

void UiView::SetOpenGLWindowMousePos(ImVec2 mousePos)
{
	OpenGLWindowMousePos = mousePos;
}

ImVec2 UiView::GetOpenGLWindowMousePos()
{
	return OpenGLWindowMousePos;
}
