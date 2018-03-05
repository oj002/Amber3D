#include "skyBox.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Amber3D
{
	SkyBox::SkyBox(std::vector<std::string> faces)
	{
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		// skybox VAO
		GLCall(glGenVertexArrays(1, &skyboxVAO));
		GLCall(glGenBuffers(1, &skyboxVBO));
		GLCall(glBindVertexArray(skyboxVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

		GLCall(glGenTextures(1, &m_textureID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
				stbi_image_free(data);
			}
			else
			{
				gLog.warning() << "Cubemap texture failed to load at path: " << faces[i];
				stbi_image_free(data);
			}
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		m_source.VertexSource =
			"#version 330 core\n"
			"layout(location = 0) in vec3 aPos;\n"
			"\n"
			"out vec3 TexCoords;\n"
			"\n"
			"uniform mat4 projection;\n"
			"uniform mat4 view;\n"
			"\n"
			"void main()\n"
			"{\n"
			"TexCoords = aPos;\n"
			"vec4 pos = projection * view * vec4(aPos, 1.0);\n"
			"gl_Position = pos.xyww;\n"
			"}\n";

		m_source.FragmentSource =
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"\n"
			"in vec3 TexCoords;\n"
			"\n"
			"uniform samplerCube skybox;\n"
			"\n"
			"void main()\n"
			"{\n"
			"FragColor = texture(skybox, TexCoords);\n"
			"}\n";

		m_rendererID = CreateShader(m_source.VertexSource, m_source.FragmentSource);
		GLCall(glUseProgram(m_rendererID));
		GLCall(glUniform1i(glGetUniformLocation(m_rendererID, "skybox"), 0));
	}

	void SkyBox::draw(const Camera& cam, glm::mat4 projection)
	{
		GLCall(glDepthFunc(GL_LEQUAL));
		GLCall(glUseProgram(m_rendererID));
		glm::mat4 view = glm::mat4(glm::mat3(cam.getViewMatrix())); // remove translation from the view matrix
		GLCall(glUniformMatrix4fv(glGetUniformLocation(m_rendererID, "view"), 1, GL_FALSE, glm::value_ptr(view)));
		GLCall(glUniformMatrix4fv(glGetUniformLocation(m_rendererID, "projection"), 1, GL_FALSE, glm::value_ptr(projection)));


		GLCall(glBindVertexArray(skyboxVAO));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		GLCall(glDepthFunc(GL_LESS));
	}

	unsigned int SkyBox::CompileShader(unsigned int type, const std::string & sourc)
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

	unsigned int SkyBox::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
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
}