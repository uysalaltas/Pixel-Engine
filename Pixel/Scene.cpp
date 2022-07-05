#include "Scene.h"

#include <vector>

Scene::Scene(std::vector<ObjectStructure*>& objectStructures, std::vector<Model*>& objectModels)
	: m_objectStructures(objectStructures)
	, m_objectModels(objectModels)
{
	for (int j = 0; j <= slices; ++j) {
		for (int i = 0; i <= slices; ++i) {
			float x = (float)i * 10;
			float y = (float)j * 10;
			float z = 0;

			vertices.push_back(
				Vertex{
					glm::vec3(x, y, z),
					glm::vec3(0.8f, 0.3f, 0.02f)
				});
		}
	}

	for (int j = 0; j < slices; ++j) {
		for (int i = 0; i < slices; ++i) {

			int row1 = j * (slices + 1);
			int row2 = (j + 1) * (slices + 1);

			indices.push_back(row1 + i);
			indices.push_back(row1 + i + 1);
			indices.push_back(row1 + i + 1);
			indices.push_back(row2 + i + 1);

			indices.push_back(row2 + i + 1);
			indices.push_back(row2 + i);
			indices.push_back(row2 + i);
			indices.push_back(row1 + i);
		}
	}

	//axis = new Renderer(verticesAxis, indicesAxis, emptyTexture);
}

void Scene::SceneUpdate(Shader& shader, Camera& camera)
{
	//axis->Clear();

	//glm::mat4 mvpAxisLine = camera.GetProjMatrix() * camera.GetViewMatrix() * modelPlatform;
	//shader.SetUniformMat4f("u_MVP", mvpAxisLine);
	//axis->DrawLine(shader);

	for (int i = 0; i < m_objectStructures.size(); i++)
	{
		if (!m_objectStructures[i]->modelDefined) {
			Model* model = new Model(m_objectStructures[i]);
			m_objectModels.push_back(model);
			m_objectStructures[i]->modelDefined = true;
		}
	}

	for (int i = 0; i < m_objectModels.size(); i++)
	{
		shader.Bind();
		glm::mat4 mvp = camera.GetProjMatrix() * camera.GetViewMatrix() * m_objectStructures[i]->objModel;
		shader.SetUniformMat4f("u_MVP", mvp);
		m_objectModels[i]->Draw(shader, GL_FILL);
	}
}
