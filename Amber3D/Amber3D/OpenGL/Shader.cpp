#include "Shader.hpp"

namespace Amber3D
{
	Shader::Shader(const std::string & filepath)
		: m_filePath(filepath)
		, m_rendererID(0)
	{
		ShaderProgramSource source = PaseShader(filepath);
		m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_rendererID));
	}

	void Shader::bind() const
	{
		GLCall(glUseProgram(m_rendererID));
	}

	void Shader::unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::setFloat(const std::string & name, float v)
	{
		GLCall(glUniform1f(getUniformLocation(name), v));
	}

	void Shader::setInt(const std::string & name, int v)
	{
		GLCall(glUniform1i(getUniformLocation(name), v));
	}

	void Shader::setVec2(const std::string & name, const glm::vec2 & vec)
	{
		GLCall(glUniform2f(getUniformLocation(name), vec.x, vec.y));
	}

	void Shader::setVec3(const std::string & name, const glm::vec3 & vec)
	{
		GLCall(glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z));
	}

	void Shader::setVec4(const std::string & name, const glm::vec4 & vec)
	{
		GLCall(glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
	}

	void Shader::setMat4(const std::string & name, const glm::mat4 & mat)
	{
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
	}

	ShaderProgramSource Shader::PaseShader(const std::string& filePath)
	{
		std::ifstream fin(filePath);
		if (!fin.is_open())
		{
			gLog.warning() << "shader '" << filePath << "' doesn't exist\n";
		}

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (std::getline(fin, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string & sourc)
	{
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = sourc.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int lenght;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
			char* message = (char*)alloca(lenght * sizeof(char));
			GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
			std::cout << "Failed to compile Shader: " << message << std::endl;
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}

	unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
	{
		GLCall(unsigned int program = glCreateProgram());
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}


	int Shader::getUniformLocation(const std::string & name)
	{
		if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		{
			return m_uniformLocationCache[name];
		}
		GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
		if (location == -1)
		{
			std::cout << "Warning: uniform '" << name << "' doesn't exist\n";
		}

		m_uniformLocationCache[name] = location;
		return location;
	}

}