#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "mesh.h"
#include "pgr.h"

class ObjLoader {
  public:
    static bool loadOBJ(
        const std::string& path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals,
        std::vector<SubMesh>& subMeshes);


    static bool loadHardcode(const float* positions, const float* normals, const float* uvs,
                            const int vertex_count, const std::string& texturePath,
                            std::vector<glm::vec3>& out_vertices,
                            std::vector<glm::vec2>& out_uvs,
                            std::vector<glm::vec3>& out_normals,
                            std::vector<SubMesh>& subMeshes);

private:
    static std::unordered_map<std::string, Material> loadMTL(std::string& path);
};

