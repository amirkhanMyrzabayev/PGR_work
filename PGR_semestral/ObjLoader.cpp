#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool ObjLoader::loadOBJ(
    const std::string& path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals) {

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        return false;
    }

    std::string line;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        else if (type == "vt") {
            glm::vec2 uv_array;
            iss >> uv_array.x >> uv_array.y;
            temp_uvs.push_back(uv_array);
        }
        else if (type == "vn") {
            glm::vec3 normal_vertex;
            iss >> normal_vertex.x >> normal_vertex.y >> normal_vertex.z;
            temp_normals.push_back(normal_vertex);
        }
        else if (type == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash;
            
            for (int i = 0; i < 3; i++) {
                iss >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
                vertexIndex[i]--;
                uvIndex[i]--;
                normalIndex[i]--;
                out_vertices.push_back(temp_vertices[vertexIndex[i]]);
                out_uvs.push_back(temp_uvs[uvIndex[i]]);
                out_normals.push_back(temp_normals[normalIndex[i]]);
            }
        }
    }
    return true;
}