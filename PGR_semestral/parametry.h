#pragma once
#include <string>
#include <vector>
#include "pgr.h"

struct ObjectSetup {
	std::string path;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct StaticCamera {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
};

const StaticCamera STATIC_CAMERAS[2] = {
	{ glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
	{ glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0001f) }
};

const std::vector<ObjectSetup> SCENE_OBJECTS_SETUP = {
	{ "Assets/BarStand/BarStandModel.txt",
	glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(90.0f)), glm::vec3(1.0f) },

	{ "Assets/cartoon_building/cartoon_building.txt",
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.05f) },

	{ "Assets/bezdomovec/bezdomovecModel.txt",
	glm::vec3(4.5f, 0.0f, -0.2f), glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), glm::vec3(2.0f) }
};



