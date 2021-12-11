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

void UiView::DrawUiFrame(glm::mat4& proj, glm::mat4& view, std::vector<ObjectStructure>& model, unsigned int frameTexture)
{
	ImGui::Begin("Objects");
	if (ImGui::Button("Add Object"))
	{
		std::cout << "Button Clicked " << " " << std::endl;

		OPENFILENAME ofn;
		wchar_t szFile[260];
		const wchar_t *filter = L"All\0*.*\0Text\0*.TXT\0";
		HWND hwnd = NULL;
		HANDLE hf;
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&ofn) == TRUE) 
		{
			char output[256];
			sprintf_s(output, "%ws", szFile);
			printf(" %s\n", output);

			ObjectStructure obj;
			obj.path = output;
			//obj.modelColor = glm::vec3(1.0f, 1.0f, 0.2f);
			//obj.modelLineColor = glm::vec3(0.2f, 0.2f, 0.2f);

			char* token = NULL;
			char* name = NULL;
			char* next_token = NULL;
			const char* delimeter = "\\";
			token = strtok_s(output, delimeter, &next_token);
			while (token != NULL) {
				printf("%s\n", token);
				name = token;
				token = strtok_s(NULL, delimeter, &next_token);
			}
			printf("TOKEN %s\n", name);
			obj.name = name;
			model.push_back(obj);

			//hf = CreateFile(ofn.lpstrFile,
			//	GENERIC_READ,
			//	0,
			//	(LPSECURITY_ATTRIBUTES)NULL,
			//	OPEN_EXISTING,
			//	FILE_ATTRIBUTE_NORMAL,
			//	(HANDLE)NULL);
		}
	}

	ImGui::Separator();
	std::vector<bool> selection(model.size(), false);
	for (int n = 0; n < model.size(); n++)
	{
		char buf[256];
		sprintf_s(buf, model[n].name.c_str());
		if (ImGui::Selectable(buf, selection[n]))
		{
			std::cout << "Selected Object: " << selection[n] << std::endl;
			std::fill(selection.begin(), selection.end(), 0);
			//memset(selection, 0, sizeof(selection));
			if (selection[n] == 1)
				selection[n] == 0;
			else
				selection[n] == 1;
			selected = n;
		}
	}
	ImGui::End();

	ImGui::Begin("Transform");
	ImGui::SliderFloat3("Translation", &model[selected].objTranslation.x, 0.0f, 200.0f);
	ImGui::SliderFloat3("Rotation", &model[selected].objRotation.x, 0, 360);
	model[selected].objModel = glm::translate(glm::mat4(1.0f), model[selected].objTranslation);
	model[selected].objModel = glm::rotate(model[selected].objModel, glm::radians(model[selected].objRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model[selected].objModel = glm::rotate(model[selected].objModel, glm::radians(model[selected].objRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model[selected].objModel = glm::rotate(model[selected].objModel, glm::radians(model[selected].objRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Collider", &model[selected].AABB);
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
			glm::value_ptr(model[selected].objModel)
		);
		if (ImGuizmo::IsUsing)
		{
			model[selected].objTranslation = glm::vec3(model[selected].objModel[3]);
		}
		//ImVec2 screen_pos = ImVec2( (ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x) , (ImGui::GetCursorScreenPos().y - ImGui::GetMousePos().y));
		//SetOpenGLWindowMousePos(screen_pos);
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
