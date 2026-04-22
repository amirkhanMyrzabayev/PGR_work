#include "MeshManager.h"
#include "ObjLoader.h"
#include "mesh.h"
#include "hardcodeObject.h"
#include <iostream>

MeshManager::~MeshManager() {
	for (auto mesh : meshes) {
		glDeleteVertexArrays(1, &(mesh.second->vao));
		glDeleteBuffers(1, &(mesh.second->vbo));
		delete mesh.second;
	}
	meshes.clear();
}

Mesh* MeshManager::getMesh(const std::string& filePath, GLuint shaderProgram) {
	if (meshes.find(filePath) != meshes.end()) {
		return meshes[filePath];
	}
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uv_array;
	std::vector<glm::vec3> normals;
	std::vector<SubMesh> subMeshes;
	if (filePath == "HARD") ObjLoader::loadHardcode(_positions, _normals, _uvs, HARDCODED_VERTEX_COUNT,
													vertices, uv_array, normals, subMeshes);
	else ObjLoader::loadOBJ(filePath, vertices, uv_array, normals, subMeshes);


	size_t _numVertices = vertices.size();
	std::vector<VertexData> vertexData;
	for (size_t i = 0; i < _numVertices; i++) {
		VertexData data = { vertices[i], normals[i], uv_array[i]};
		vertexData.push_back(data);
	}
	Mesh* newMesh = new Mesh();
	newMesh->subMeshes = subMeshes;
	//newMesh->numVertices = _numVertices;
	glGenVertexArrays(1, &(newMesh->vao));
	glGenBuffers(1, &(newMesh->vbo));
	glBindVertexArray(newMesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), vertexData.data(), GL_STATIC_DRAW);
	GLint posLoc = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	GLint normalLoc = glGetAttribLocation(shaderProgram, "normal");
	GLint uvLoc = glGetAttribLocation(shaderProgram, "texCoord");
	if (uvLoc != -1) {
		glEnableVertexAttribArray(uvLoc);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 2));
	}
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(VertexData::position)));
	glBindVertexArray(0);
	meshes[filePath] = newMesh;
	return newMesh;
}