#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utils.h"
#include "Object.h"

class Model
{
public:
	Model(ObjectStructure& object);
	~Model();
	void Draw(Shader& shader, GLenum mode);

private:
	std::string directory;
	std::vector<Renderer*> meshes;
	std::vector<Renderer*> meshesAABB;
	std::vector<Texture> textures_loaded;

	std::vector<float> aabbVertices;
	float xMin, yMin, zMin = 200;
	float xMax, yMax, zMax = 0;

	Utils t1;

	ObjectStructure& modelObj;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Renderer* processMesh(aiMesh* mesh, const aiScene* scene);
	Renderer* createAABB();
	
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};