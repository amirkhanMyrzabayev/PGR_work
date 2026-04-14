#pragma once
#include "pgr.h"



struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::string name;
	float shininess;
};

struct SubMesh {
	unsigned int startIndex;
	unsigned int numVertices;
	Material material;
};

struct Mesh {
	GLuint vao;
	GLuint vbo;
	std::vector<SubMesh> subMeshes;
};

struct VertexData {
	glm::vec3 position;
	glm::vec3 normal;
};
