/**

    @file      ObjLoader.h
    @brief     Class that loads OBJ files.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once


#include <vector>
#include <string>
#include <unordered_map>
#include "mesh.h"
#include "pgr.h"

/// @brief Helper class to load 3D models and make basic shapes.
class ObjLoader {
public:
    /// @brief Loads an OBJ file and splits the parts by material.
    /// @param path Path to the .obj file.
    /// @param out_vertices Vector to store vertex positions.
    /// @param out_uvs Vector to store UV texture coordinates.
    /// @param out_normals Vector to store normal vectors.
    /// @param subMeshes Vector to store the loaded sub-meshes.
    /// @return True if it loads successfully, false if it fails.
    static bool loadOBJ(
        const std::string& path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals,
        std::vector<SubMesh>& subMeshes);

    /// @brief Creates mesh data from simple C arrays instead of a file.
    /// @param positions Array of 3D positions (x, y, z).
    /// @param normals Array of 3D normals (x, y, z).
    /// @param uvs Array of 2D UV coordinates (u, v).
    /// @param vertex_count Number of vertices in the arrays.
    /// @param texturePath Path to the texture file.
    /// @param out_vertices Vector to store vertex positions.
    /// @param out_uvs Vector to store UV texture coordinates.
    /// @param out_normals Vector to store normal vectors.
    /// @param subMeshes Vector to store the created sub-mesh.
    /// @return True if created successfully.
    static bool loadHardcode(const float* positions, const float* normals, const float* uvs,
        const int vertex_count, const std::string& texturePath,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals,
        std::vector<SubMesh>& subMeshes);

    /// @brief Creates a 3D cylinder shape standing on the Y-axis.
    /// @param radius Radius of the cylinder.
    /// @param height Height of the cylinder.
    /// @param sectorCount How many slices the cylinder has (smoothness).
    /// @param texturePath Path to the texture file.
    /// @param out_vertices Vector to store vertex positions.
    /// @param out_uvs Vector to store UV texture coordinates.
    /// @param out_normals Vector to store normal vectors.
    /// @param subMeshes Vector to store the created sub-mesh.
    /// @return True if created successfully.
    static bool loadProceduralCylinder(const float radius, const float height, const size_t sectorCount,
        const std::string& texturePath,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals,
        std::vector<SubMesh>& subMeshes);

private:
    /// @brief Loads a material (.mtl) file and its textures.
    /// @param path Path to the .mtl file.
    /// @return A map of material names to Material objects.
    static std::unordered_map<std::string, Material> loadMTL(std::string& path);
};