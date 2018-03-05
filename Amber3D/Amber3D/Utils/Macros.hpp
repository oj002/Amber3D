#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <glad\glad.h>

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace Amber3D
{
	#ifdef _DEBUG
	#define DEBUG_CALL(x) x;
	#define GLCall(x) \
	while (glGetError()){}\
	x;\
	if(true)\
	{\
		bool error = false; \
		while (GLenum errorCode = glGetError())\
		{\
			std::string errorMsg;\
			switch (errorCode)\
			{\
				case GL_INVALID_ENUM:					errorMsg = "INVALID_ENUM"; break;\
				case GL_INVALID_VALUE:					errorMsg = "INVALID_VALUE"; break;\
				case GL_INVALID_OPERATION:				errorMsg = "INVALID_OPERATION"; break;\
				case GL_OUT_OF_MEMORY:					errorMsg = "OUT_OF_MEMORY"; break;\
				case GL_INVALID_FRAMEBUFFER_OPERATION:	errorMsg = "INVALID_FRAMEBUFFER_OPERATION"; break;\
				default:								errorMsg = std::to_string(errorCode); break;\
			}\
			std::cerr << "[OpenGL Error] (" << errorMsg << "): " << #x << " " << __FILE__ << ':' << __LINE__ << '\n'; \
			error = true; \
		}\
		if (error) { __debugbreak(); }\
	}

	#else
	#define GLCall(x) x;
	#define DEBUG_CALL(x)
	#endif

} // namespace Amber3D