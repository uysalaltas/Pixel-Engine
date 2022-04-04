#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "Model.h"

class Scene
{
public:
	Scene(std::vector<ObjectStructure*>& objectStructures, std::vector<Model*>& objectModels);
	~Scene() {
		delete axis;
	}

	void SceneUpdate(Shader& shader, Camera& camera);

private:
	std::vector<ObjectStructure*>& m_objectStructures;
	std::vector<Model*>& m_objectModels;
	std::vector<Vertex> verticesAxis = {
		Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(0.0f, 0.0f, 1.0f)},
		Vertex{glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f)}
	};
	std::vector<GLuint> indicesAxis = {
		0, 1,
		2, 3,
		4, 5
	};
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> emptyTexture;

	float platfromSize = 200.0f;
	int slices = platfromSize / 10;
	glm::mat4 modelPlatform = glm::mat4(1.0f);

	Renderer *axis;
};