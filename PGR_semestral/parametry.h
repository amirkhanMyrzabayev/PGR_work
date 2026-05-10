/**

    @file      parametry.h
    @brief     Header for constants and parameters.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once
#include <vector>
#include <string>
#include "pgr.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

// Scene constants
constexpr const int WIN_WIDTH = 1024;
constexpr const int WIN_HEIGHT = 1024;
constexpr const char* WIN_TITLE = "Lonely scientist";


constexpr const float MIN_X = -50.0f;
constexpr const float MAX_X = 50.0f;
constexpr const float MIN_Z = -50.0f;
constexpr const float MAX_Z = 50.0f;
constexpr const float MIN_Y = -5.0f;
constexpr const float MAX_Y = 25.0f;
constexpr const float TILE_SIZE = 10.0f;

constexpr const float CAMERA_PADDING = 2.0f;

// Light limits
constexpr const int MAX_DIR_LIGHTS = 5;
constexpr const int MAX_POINT_LIGHTS = 5;
constexpr const int MAX_SPOT_LIGHTS = 5;

// Shader and Asset paths
const std::string mainLightShaderName = "Shaders/3d_light_pixel";
const std::string tilePath = "Assets/snowLand/snowLand.obj";
// Keywords for specific objects
const std::string HARDCODED_OBJ = "HARD";
const std::string FIRE_SPRITE_OBJ = "HARD_FIRE";
const std::string CLOCK_HUD = "HUD";
const std::string CYLINDER = "CYLINDER";


constexpr const float FOG_START = 20.0f;
constexpr const float FOG_END = 50.0f;
const glm::vec3 FOG_COLOR = glm::vec3(0.6f, 0.7f, 0.75f);


constexpr const float CYLINDER_RADIUS = 1.0f;
constexpr const float CYLINDER_HEIGHT = 4.0f;
constexpr const size_t CYLINDR_SECTOR_COUNT = 36;

const glm::vec3 DAY_STATUSBAR_POSITION = glm::vec3(0.8f, 0.8f, 0.0f);


/// @brief Basic settings needed to load and place any 3D object.
struct ObjectSetup {
	std::string path;        ///< Path to the model file.
	std::string shaderPath;  ///< Path to the shader files.
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool isTexAnim = false;  ///< If true, the texture will slide/move over time.
	int pointLightIndex = -1; ///< Index of an attached point light (-1 if none).
	int spotLightIndex = -1;  ///< Index of an attached spotlight (-1 if none).
};

/// @brief Settings for objects that move in an elliptical orbit.
struct AnimatedObjectSetup : public ObjectSetup {
	float radiusX;           ///< Horizontal width of the movement circle.
	float radiusZ;           ///< Depth length of the movement circle.
	float speed;
	float animationOffset = 0.0f; ///< Distance for the following sprite (like fire).
};

/// @brief Settings for 2D animated sprites.
struct SpriteObjectSetup : public ObjectSetup {
	size_t columns;          ///< Number of horizontal frames in the sprite sheet.
	size_t rows;             ///< Number of vertical frames in the sprite sheet.
	float fps;               ///< Speed of the animation.
};

/// @brief Pre-defined camera position and orientation.
struct StaticCamera {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
};


// Global Data Arrays
const StaticCamera STATIC_CAMERAS[2] = {
	{ glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
	{ glm::vec3(9.0f, 13.0f, 8.0f), glm::vec3(-0.2f, -0.2f, -0.2f), glm::vec3(0.0f, 1.0f, 0.0f) }
};


const std::vector<glm::vec3> CAMERA_PATH = {
	glm::vec3(0.0f, 5.0f,  10.0f), 
	glm::vec3(10.0f, 2.0f,  0.0f), 
	glm::vec3(0.0f, 8.0f, -10.0f), 
	glm::vec3(-10.0f, 3.0f,  0.0f),
	glm::vec3(10.0f, 2.0f,  0.0f),
	glm::vec3(0.0f, 5.0f,  10.0f)
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
		glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(4.55f, 1.2f, 0.5f),
		//	linearDecay						quadraticDecay			constantDecay(default=1.0f) 
			0.18f,							0.128f
	}
};

const std::vector<SpotLightSetup> SPOT_LIGHTS_SETUP = {
	{
		//  ambient						diffuse
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		//	specular					direction
		glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f),
		//  position					innerCutOffAngle			outerCutOffAngle
		glm::vec3(0.0f, 8.0f, -0.5f), glm::radians(6.5f),			glm::radians(10.5f),
		//	linearDecay					quadraticDecay				constantDecay(default=1.0f) 
		0.09f,							0.032f
	}
};



const std::vector<std::string> LIGHT_SHADERS = {
	"Shaders/3d_light_pixel", "Shaders/3d_light_pixel_time",
};

const std::vector<std::string> BORDER_OBJECTS_PATHS = {
	"Assets/Stone/Stone.obj"
};
constexpr const float STONE_COLLISION_RADIUS = 5.0f;

const ObjectSetup ARM_CYLINDER_SETUP = {
	CYLINDER, "Shaders/3d_light_pixel",
	glm::vec3(15.0f, 0.5f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f)
};

const std::vector<ObjectSetup> SCENE_OBJECTS_SETUP = {
	{ "Assets/BarStand/BarStandModel.obj", "Shaders/3d_light_pixel",
	glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(90.0f)), glm::vec3(1.0f) },

	{ "Assets/cartoon_building/cartoon_building.obj", "Shaders/3d_light_pixel",
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.05f), false, -1, 0 },

	{ "Assets/bezdomovec/bezdomovecModel.obj", "Shaders/3d_light_pixel",
	glm::vec3(4.5f, 0.0f, -0.2f), glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), glm::vec3(2.0f) },

	{ "Assets/trashbin/trashbin.obj", "Shaders/3d_light_pixel",
	glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f) },


	{ "Assets/kadibouda/kadibouda.obj", "Shaders/3d_light_pixel",
	glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0125f) },

	{ "Assets/gitara/gitara.obj", "Shaders/3d_light_pixel",
	glm::vec3(4.4f, 1.1f, 0.0f), glm::vec3(0.0f, glm::radians(90.0f), glm::radians(-90.0f)), glm::vec3(0.005f)},

	{ "Assets/Cat/Cat.obj", "Shaders/3d_light_pixel_time",
	glm::vec3(5.0f, 0.0f, 2.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.0125f)},

	{ "Assets/lighter/lighter.obj", "Shaders/3d_light_pixel",
	glm::vec3(4.5f, 1.1f, 0.5f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.125f), false, 0},

	{ HARDCODED_OBJ, "Shaders/3d_light_pixel",
	glm::vec3(4.8f, 1.1f, 0.5f), glm::vec3(0.0f), glm::vec3(0.1f), true},


	
};

const std::vector<ObjectSetup> TRANSPARENT_OBJECTS_SETUP = {
	{ "Assets/beer_bottle/beer_bottle.obj", "Shaders/3d_light_pixel",
	glm::vec3(4.2f, 1.03f, 0.5f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(1.0f)},
	{ "Assets/bottles/bottles.obj", "Shaders/3d_light_pixel",
	glm::vec3(3.6f, 1.03f, 1.0f), glm::vec3(0.0f, glm::radians(20.0f), 0.0f), glm::vec3(0.005f)},
	{ "Assets/wineBottle/wineBottle.obj", "Shaders/3d_light_pixel",
	glm::vec3(3.4f, 1.03f, 0.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.025f) }


};

const std::vector<AnimatedObjectSetup> ANIMATED_OBJECTS_SETUP = {
		
		{	//parent struct
			{
				"Assets/bixler/bixler.obj", "Shaders/3d_light_pixel",
				glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f), glm::vec3(0.05f)
			},
			// child parameters
			3.0f, 3.0f, 2.0f,  0.55f  
		}
};

const std::vector<SpriteObjectSetup> SPRITE_OBJECTS_SETUP = {
	{
		{
			FIRE_SPRITE_OBJ, "Shaders/animated_texture",
			glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(0.0f, glm::radians(90.0f), glm::radians(90.0f)), glm::vec3(1.0f)
		},
	// child parameters
	5, 3, 20.0f
	},
	{ 
		{ 
			CLOCK_HUD, "Shaders/hud_clock_texture", glm::vec3(0.8f, -0.8f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1f)
		},
			5, 3, 20.0f 
	}
};



const std::vector<std::string> SKYBOX_FACES = {
	"Assets/Cubemaps/sky/1_posx.png", "Assets/Cubemaps/sky/1_negx.png",
	"Assets/Cubemaps/sky/1_posy.png", "Assets/Cubemaps/sky/1_negy.png",
	"Assets/Cubemaps/sky/1_posz.png", "Assets/Cubemaps/sky/1_negz.png"
};

