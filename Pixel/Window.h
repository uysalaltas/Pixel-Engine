#pragma once


class Window
{
public:
	static Window& Get()
	{
		return s_Window;
	}

	float WIDTH = 900;
	float HEIGHT = 900;

private:
	Window() {}
	static Window s_Window;
};

Window Window::s_Window;