#include "../Logger/Logger.hpp"
#include "../Utils/Macros.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

namespace Amber3D
{
	class SkyBox
	{
	public:
		explicit SkyBox(std::vector<std::string> faces = { "right.png", "left.png", "top.png", "bottom.png", "back.png", "front.png" });

		void draw(const Camera& cam, glm::mat4 projection);
		/*
			Call draw as the last draw call to get maximum performance
			Call draw as the first draw call to work with blending
		*/

	private:
		unsigned int CompileShader(unsigned int type, const std::string & sourc);
		unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

	private:
		unsigned int skyboxVAO, skyboxVBO;
		unsigned int m_textureID, m_rendererID;

		ShaderProgramSource m_source;
	};
}  // namespace Amber3D