/**

    @file      MeshManager.h
    @brief     Manager class to load, store, and reuse 3D models and shapes.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once

#include <string>
#include <unordered_map>
#include "pgr.h"
#include "mesh.h"

/// @brief Manager class to load, store, and reuse 3D models and shapes.
class MeshManager
{
public:
	~MeshManager();

	/// @brief Gets a mesh from memory, or loads/creates it if it hasn't been loaded yet.
	/// @param filePath The path to the .obj file, or a special keyword (like "HARD", "HUD", or "CYLINDER") for built-in shapes.
	/// @param shaderProgram The OpenGL ID of the shader program, used to link vertex attributes (position, normal, UV).
	/// @return A pointer to the ready-to-use Mesh object.
	Mesh* getMesh(const std::string& filePath, GLuint shaderProgram);

private:
	/// Map keeping track of loaded meshes so we don't build them twice (Key: path or keyword, Value: Mesh pointer).
	std::unordered_map<std::string, Mesh*> meshes;
};