#pragma once
#include "../Logger/Logger.hpp"
#include "../Utils/Macros.hpp"
#include <array>
#include <functional>


namespace Amber3D
{
	struct Window
	{
	public:
		Window(int width, int height, const char* title, bool fullscreen = false, std::function<void()> glfwWindowHints = []() { glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); });
		~Window();

		void pollEvent();
		inline void clear() { GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); }
		int getKey(int key) { return glfwGetKey(pWindow, key); }
		inline void swapBuffers() { glfwSwapBuffers(pWindow); }

		inline bool is_open() const { return glfwWindowShouldClose(pWindow) == GLFW_FALSE; }

		inline void close() { glfwSetWindowShouldClose(pWindow, GLFW_TRUE); }
		inline void setClearColor(float r, float g, float b) { GLCall(glClearColor(r, g, b, 1.0)); }

		inline void resize(int newWidth, int newHeight) { GLCall(glViewport(0, 0, newWidth, newHeight)); width = newWidth; height = newHeight; }

	public:
		GLFWwindow* pWindow;
		int width, height;
	};
}  // namespace Amber3D