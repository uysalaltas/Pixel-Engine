#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

class Model
{
public:
	Model(std::string path);
	~Model();
	void Draw(Shader& shader);

private:
	std::string directory;
	std::vector<Renderer*> meshes;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Renderer* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
