#include "UiView.h"

UiView::UiView(GLFWwindow* window, Camera* camera)
	: m_camera(camera)
{
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;


	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
}

void UiView::DrawUiFrame(glm::mat4& proj, glm::mat4& view, std::vector<ObjectStructure*>& model, unsigned int frameTexture)
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

			ObjectStructure* obj = new ObjectStructure();
			obj->path = output;
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
			obj->name = name;
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
		sprintf_s(buf, model[n]->name.c_str());
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
	ImGui::Checkbox("Full Window", &_fullWindow);
	ImGui::SliderFloat3("Translation", &model[selected]->objTranslation.x, 0.0f, 200.0f);
	ImGui::SliderFloat3("Rotation", &model[selected]->objRotation.x, 0, 360);
	model[selected]->objModel = glm::translate(glm::mat4(1.0f), model[selected]->objTranslation);
	model[selected]->objModel = glm::rotate(model[selected]->objModel, glm::radians(model[selected]->objRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model[selected]->objModel = glm::rotate(model[selected]->objModel, glm::radians(model[selected]->objRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model[selected]->objModel = glm::rotate(model[selected]->objModel, glm::radians(model[selected]->objRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Collider", &model[selected]->AABB);
	ImGui::End();

	
	//if (!_fullWindow)
	//{
	//	ImGui::SetNextWindowSize(ImVec2(WIDTH * 0.7f, HEIGHT * 0.7f));
	//}
	//else
	//{
	//	ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
	//}
	//ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);
	ImGui::Begin("Scene");
	{
		ImGui::BeginChild("GameRender");
		m_camera->SetFOV(ImGui::GetContentRegionAvail().x / ImGui::GetContentRegionAvail().y);
		ImGui::Image((ImTextureID)frameTexture, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		
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
			glm::value_ptr(model[selected]->objModel)
		);
		if (ImGuizmo::IsUsing)
		{
			model[selected]->objTranslation = glm::vec3(model[selected]->objModel[3]);
		}
	}
	ImGui::EndChild();
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
