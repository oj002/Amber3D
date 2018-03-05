#include "Window.hpp"

namespace Amber3D
{
	Window::Window(int width, int height, const char * title, bool fullscreen, std::function<void()> glfwWindowHints)
		: pWindow(nullptr)
		, width(width)
		, height(height)
	{
		if (glfwInit() == GL_FALSE)
		{
			gLog << "Failed to initialize GLFW";
			exit(0);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHints();

		#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
		#endif

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		/* Create a windowed mode window and its OpenGL context */
		if (fullscreen)
		{
			pWindow = glfwCreateWindow(mode->width, mode->height, title, primaryMonitor, nullptr);
			this->width = mode->width;
			this->height = mode->height;
		}
		else
		{
			pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
		}

		if (pWindow == nullptr)
		{
			gLog.fatal() << "Failed to create GLFW window";
			glfwTerminate();
			exit(0);
		}
		glfwMakeContextCurrent(pWindow);

		int versionMajor, versionMinor, versionRevision;
		glfwGetVersion(&versionMajor, &versionMinor, &versionRevision);
		gLog.info() << "Initialized GLFW " << versionMajor << "." << versionMinor << " rev. " << versionRevision;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			gLog.fatal() << "Failed to init GLAD";
			glfwTerminate();
			exit(-1);
		}

		glEnable(GL_DEPTH_TEST);
	}

	Window::~Window()
	{
		glfwDestroyWindow(pWindow);
		glfwTerminate();
	}

	void Window::pollEvent()
	{
		glfwGetFramebufferSize(pWindow, &width, &height);
		GLCall(glViewport(0, 0, width, height));
		glfwPollEvents();
	}
}