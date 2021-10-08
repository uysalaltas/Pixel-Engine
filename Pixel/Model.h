#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Renderer.h"

class Model
{
public:
	Model(std::string path);
	void Draw(Shader& shader);

private:
	std::string directory;
	std::vector<Renderer> meshes;
	std::vector<MeshData> meshesData;
	
	bool firstLoop = true;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	MeshData processMesh(aiMesh* mesh, const aiScene* scene);
	//vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

