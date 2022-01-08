#include "GLWindow.h"


GLWindow::GLWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Pixel Slicer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
	
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mouse_callback_static);
	glfwSetScrollCallback(window, scroll_callback_static);
	glfwSetWindowSizeCallback(window, window_size_callback_static);
	
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
}

GLWindow::~GLWindow()
{
	std::cout << "Destroyed Window" << std::endl;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLWindow::InitCamera(Camera* camera, FrameBuffer* frameBuffer)
{
	m_camera = camera;
	m_frameBuffer = frameBuffer;
}

void GLWindow::Bind()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::Unbind()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::window_size_callback_static(GLFWwindow* window, int width, int height)
{
	GLWindow* that = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	that->window_size_callback(window, width, height);
}

void GLWindow::mouse_callback_static(GLFWwindow* window, double xpos, double ypos)
{
	GLWindow* that = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	that->mouse_callback(window, xpos, ypos);
}

void GLWindow::scroll_callback_static(GLFWwindow* window, double xpos, double ypos)
{
	GLWindow* that = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	that->scroll_callback(window, xpos, ypos);
}

void GLWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

		m_camera->ArcBallCamera((lastMousePosRightClick.x - xpos), (lastMousePosRightClick.y - ypos));
		lastMousePosRightClick.x = xpos;
		lastMousePosRightClick.y = ypos;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (firstMouseClick)
		{
			lastMousePosRightClick.x = xpos;
			lastMousePosRightClick.y = ypos;
			firstMouseClick = false;
		}
		//std::cout << "LAST MOUSE POS: " << glm::to_string(camera.GetLookAt()) << std::endl;
		m_camera->PanCamera(lastMousePosRightClick - currentMousePosClick);
		lastMousePosRightClick.x = xpos;
		lastMousePosRightClick.y = ypos;
	}
	else
	{
		firstMouseClick = true;
	}
}

void GLWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_camera->ProcessMouseScroll(yoffset);
}

void GLWindow::window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	m_frameBuffer->RescaleFrameBuffer(width, height);
}
