#pragma once
#include <glm/glm.hpp>

struct ObjectStructure
{
	ObjectStructure(){}

	bool modelDefined = false;
	std::string path;
	std::string name;
	glm::vec3 modelColor;
	glm::vec3 modelLineColor;
	glm::mat4 objModel = glm::mat4(1.0f);
	glm::vec3 objRotation = glm::vec3(0.0f);
	glm::vec3 objTranslation = glm::vec3(0.0f);
};