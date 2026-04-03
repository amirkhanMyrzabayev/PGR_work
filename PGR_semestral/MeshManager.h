#pragma once
#include <string>
#include <unordered_map>
#include "pgr.h"
#include "mesh.h"

class MeshManager
{
public:
	~MeshManager();
	Mesh* getMesh(const std::string& filePath, GLuint shaderProgram);
	 
private:
	std::unordered_map<std::string, Mesh*> meshes;
};

