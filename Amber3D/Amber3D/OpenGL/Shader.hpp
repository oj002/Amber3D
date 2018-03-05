#pragma once

#include <string>

#include <fstream>
#include <sstream>

#include "../Logger/Logger.hpp"
#include "../Utils/Macros.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm\glm.hpp>
#include <unordered_map>

namespace Amber3D
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	public:
		explicit Shader(const std::string& filepath);
		~Shader();


		void bind() const;
		void unbind()const;

		void setFloat(const std::string& name, float v);
		void setInt(const std::string& name, int v);

		void setVec2(const std::string& name, const glm::vec2& vec);
		void setVec3(const std::string& name, const glm::vec3& vec);
		void setVec4(const std::string& name, const glm::vec4& vec);
		void setMat4(const std::string& name, const glm::mat4& mat);

	private:
		ShaderProgramSource PaseShader(const std::string& filePath);
		unsigned int CompileShader(unsigned int type, const std::string& sourc);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int getUniformLocation(const std::string& name);

	private:
		std::string m_filePath;
		unsigned int m_rendererID;
		std::unordered_map<std::string, int> m_uniformLocationCache;
	};
}  // namespace Amber3D