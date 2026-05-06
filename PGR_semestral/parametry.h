#pragma once
#include <vector>
#include <string>
#include "pgr.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


constexpr const int WIN_WIDTH = 1024;
constexpr const int WIN_HEIGHT = 1024;
constexpr const char* WIN_TITLE = "PRG_Semestral";


constexpr const float MIN_X = -50.0f;
constexpr const float MAX_X = 50.0f;
constexpr const float MIN_Z = -50.0f;
constexpr const float MAX_Z = 50.0f;
constexpr const float MIN_Y = -5.0f;
constexpr const float MAX_Y = 25.0f;
constexpr const float TILE_SIZE = 10.0f;

constexpr const float CAMERA_PADDING = 2.0f;

constexpr const int MAX_DIR_LIGHTS = 5;
constexpr const int MAX_POINT_LIGHTS = 5;
constexpr const int MAX_SPOT_LIGHTS = 5;


const std::string mainLightShaderName = "Shaders/3d_light_pixel";
const std::string tilePath = "Assets/snowLand/snowLand.obj";


constexpr const float FOG_START = 20.0f;
constexpr const float FOG_END = 50.0f;
const glm::vec3 FOG_COLOR = glm::vec3(0.6f, 0.7f, 0.75f);


struct FogPositions
{
	GLuint fogStartPos;
	GLuint fogEndPos;
	GLuint fogColorPos;
};


struct ObjectSetup {
	std::string path;
	std::string shaderPath;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool isTexAnim = false;
};

struct AnimatedObjectSetup : public ObjectSetup {
	float radiusX;
	float radiusZ;
	float speed;
	float animationOffset = 0.0f;
};

struct SpriteObjectSetup : public ObjectSetup {
	size_t columns;
	size_t rows;
	float fps;
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

const std::vector<std::string> BORDER_OBJECTS_PATHS = {
	"Assets/Stone/Stone.obj"
};
constexpr const float STONE_COLLISION_RADIUS = 5.0f;

const std::vector<ObjectSetup> SCENE_OBJECTS_SETUP = {
	{ "Assets/BarStand/BarStandModel.obj", "Shaders/3d_light_pixel",
	glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(90.0f)), glm::vec3(1.0f) },

	{ "Assets/cartoon_building/cartoon_building.obj", "Shaders/3d_light_pixel",
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.05f) },

	{ "Assets/bezdomovec/bezdomovecModel.obj", "Shaders/3d_light_pixel",
	glm::vec3(4.5f, 0.0f, -0.2f), glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), glm::vec3(2.0f) },

	{ "Assets/trashbin/trashbin.obj", "Shaders/3d_light_pixel",
	glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f) },



	{ "Assets/Cat/Cat.obj", "Shaders/3d_light_pixel",
	glm::vec3(5.0f, 0.0f, 2.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.0125f)},

	{ "HARD", "Shaders/3d_light_pixel",
	glm::vec3(4.8f, 1.05f, 0.5f), glm::vec3(0.0f), glm::vec3(0.1f), true}
};

const std::vector<AnimatedObjectSetup> ANIMATED_OBJECTS_SETUP = {
		//parent struct
		{ { "Assets/bixler/bixler.obj", "Shaders/3d_light_pixel",
		glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(0.05f)},
		// child parameters
		3.0f, 3.0f, 2.0f,  0.55f  }
};

const std::vector<SpriteObjectSetup> SPRITE_OBJECTS_SETUP = {
	{ { "HARD_FIRE", "Shaders/animated_texture",
		glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(0.0f, glm::radians(90.0f), glm::radians(90.0f)), glm::vec3(1.0f)},
		// child parameters
		5, 3, 20.0f}
};



const std::vector<DirLightSetup> DIR_LIGHTS_SETUP = {
	{
		// Sunlight
		//  ambient							diffuse
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.8f, 0.8f, 0.8f),
		//	specular						direction
		glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(-0.2f, -1.0f, -0.3f)
	},
		//PassiveLight
	{
		//  ambient							diffuse
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f),
		//	specular						direction
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, -1.0f, 0.0f)
	}
};

const std::vector<PointLightSetup> POINT_LIGHTS_SETUP = {
	{
		//  ambient							diffuse
		glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.0f),
		//	specular						position
		glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(4.0f, 1.1f, 0.5f),
		//	linearDecay						quadraticDecay			constantDecay(default=1.0f) 
			0.09f,							0.032f
	}
};

const std::vector<SpotLightSetup> SPOT_LIGHTS_SETUP = {
	{
		//  ambient						diffuse
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		//	specular					direction
		glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f),
		//  position					innerCutOffAngle			outerCutOffAngle
		glm::vec3(0.0f, 5.0f, 0.0f), glm::radians(12.5f),			glm::radians(17.5f), 
		//	linearDecay					quadraticDecay				constantDecay(default=1.0f) 
		0.09f,							0.032f
	}
};

const std::vector<std::string> SKYBOX_FACES = {
	"Assets/Cubemaps/sky/1_posx.png", "Assets/Cubemaps/sky/1_negx.png",
	"Assets/Cubemaps/sky/1_posy.png", "Assets/Cubemaps/sky/1_negy.png",
	"Assets/Cubemaps/sky/1_posz.png", "Assets/Cubemaps/sky/1_negz.png",
};

