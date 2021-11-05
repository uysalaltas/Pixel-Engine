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
	Model(std::string path, glm::vec3 modelColor);
	~Model();
	void Draw(Shader& shader, GLenum mode);

private:
	std::string directory;
	std::vector<Renderer*> meshes;
	std::vector<Texture> textures_loaded;

	glm::vec3 colorData = glm::vec3(1.0f);

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Renderer* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
