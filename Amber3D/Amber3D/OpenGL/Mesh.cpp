#include "Mesh.hpp"

namespace Amber3D
{

	Mesh::Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		: vertices(vertices)
		, indices(indices)
		, textures(textures)
	{
		GLCall(glGenVertexArrays(1, &this->VAO));
		GLCall(glGenBuffers(1, &this->VBO));
		GLCall(glGenBuffers(1, &this->EBO));

		GLCall(glBindVertexArray(this->VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW));

		// vertex positions
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
		// vertex normals
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
		// vertex texture coords
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)));

		GLCall(glBindVertexArray(0));
	}

	void Mesh::draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int emissionNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));

			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_emission")
			{
				number = std::to_string(emissionNr++);
			}

			shader.setInt(("material." + name + number).c_str(), i);
			GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
		}
		GLCall(glActiveTexture(GL_TEXTURE0));

		// draw mesh
		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
		GLCall(glBindVertexArray(0));
	}

}