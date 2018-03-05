#pragma once
#include "Shader.hpp"
#include <string>

#include "../Utils/Macros.hpp"

namespace Amber3D
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void draw(Shader& shader);

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

	private:
		unsigned int VAO, VBO, EBO;
	};
}  // namespace Amber3D