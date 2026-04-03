#pragma once
#include "pgr.h"

struct Mesh {
	GLuint vao;
	GLuint vbo;
	size_t numVertices;
};

struct VertexData {
	glm::vec3 position;
	glm::vec3 normal;
};
