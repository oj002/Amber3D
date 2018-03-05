#pragma once

#include "../Logger/Logger.hpp"
#include "Mesh.hpp"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <stb_image.h>

namespace Amber3D
{
	class Model
	{
	public:
		explicit Model(const char *path)
		{
			loadModel(path);
		}

		void draw(Shader& shader)
		{
			for (auto itr = meshes.begin(); itr != meshes.end(); itr++)
			{
				itr->draw(shader);
			}
		}

	private:
		void loadModel(const std::string& path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);

	private:
		std::vector<Texture> textures_loaded{};
		std::vector<Mesh> meshes{};
		std::string directory;

	};
} // namespace Amber3D