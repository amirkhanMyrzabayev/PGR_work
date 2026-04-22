#pragma once
#include <vector>
#include <string>
#include "pgr.h"

constexpr const int WIN_WIDTH = 1024;
constexpr const int WIN_HEIGHT = 1024;
constexpr const char* WIN_TITLE = "PRG_Semestral";

struct ObjectSetup {
	std::string path;
	std::string shaderPath;
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
	{ glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.00001f), glm::vec3(0.0f, 1.0f, 0.0f) }
};

const std::vector<ObjectSetup> SCENE_OBJECTS_SETUP = {
	{ "Assets/BarStand/BarStandModel.obj", "Shaders/3d_light",
	glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(90.0f)), glm::vec3(1.0f) },

	{ "Assets/cartoon_building/cartoon_building.obj", "Shaders/3d_light",
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.05f) },

	{ "Assets/bezdomovec/bezdomovecModel.obj", "Shaders/3d_light",
	glm::vec3(4.5f, 0.0f, -0.2f), glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), glm::vec3(2.0f) },

	{ "HARD", "Shaders/3d_light",
	glm::vec3(4.8f, 1.05f, 0.5f), glm::vec3(0.0f), glm::vec3(0.025f)}
};

const std::vector<std::string> SKYBOX_FACES = {
	"Assets/Cubemaps/sky/1_posx.png", "Assets/Cubemaps/sky/1_negx.png",
	"Assets/Cubemaps/sky/1_posy.png", "Assets/Cubemaps/sky/1_negy.png",
	"Assets/Cubemaps/sky/1_posz.png", "Assets/Cubemaps/sky/1_negz.png",
};



