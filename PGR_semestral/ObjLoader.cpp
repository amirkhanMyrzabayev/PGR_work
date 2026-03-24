#define _CRT_SECURE_NO_WARNINGS    
#include "ObjLoader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

bool ObjLoader::loadOBJ(
    const std::string& path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals) {

    // Using fopen_s as Visual Studio considers standard fopen unsafe
    FILE* file;
    if (fopen_s(&file, path.c_str(), "r") != 0 || file == nullptr) {
        std::cout << "Failed to open file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    char lineHeader[128];
    // Read the first word of the line until the End Of File
    while (fscanf(file, "%127s", lineHeader) != EOF) {

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            
            if (matches != 9) {
                std::cout << "File cannot be read by parser! Bad format!\n";
                fclose(file);
                return false;
            }

            for (int i = 0; i < 3; i++) {
                out_vertices.push_back(temp_vertices[vertexIndex[i] - 1]);
                out_uvs.push_back(temp_uvs[uvIndex[i] - 1]);
                out_normals.push_back(temp_normals[normalIndex[i] - 1]);
            }
        }
    }

    fclose(file);
    return true;
}