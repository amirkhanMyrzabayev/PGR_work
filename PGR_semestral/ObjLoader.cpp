#define _CRT_SECURE_NO_WARNINGS    
#include "ObjLoader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

bool ObjLoader::loadOBJ(
    const std::string& path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<SubMesh>& subMeshes) {

    std::string currentMaterialName = "";
    unsigned int currentStartIndex = 0;
    std::unordered_map<std::string, Material> materials;
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
        else if (strcmp(lineHeader, "mtllib") == 0) {
            char mtllibName[127];
            fscanf(file, "%127s\n", mtllibName);
            size_t slashIndex = path.find_last_of("/\\");
            std::string materialName = path.substr(0, slashIndex + 1) + mtllibName;
            materials = loadMTL(materialName);

        }
        else if (strcmp(lineHeader, "usemtl") == 0) {
            char matName[128];
            fscanf(file, "%127s\n", matName);


            // If out_vertices.size() is greater than currentStartIndex, it means 
            // we have just finished reading a bunch of faces for the previous material.
            if (out_vertices.size() > currentStartIndex) {
                SubMesh subset;
                subset.startIndex = currentStartIndex;
                subset.numVertices = out_vertices.size() - currentStartIndex;
                subset.material = materials[currentMaterialName];
                subMeshes.push_back(subset);
            }

            currentMaterialName = matName;
            currentStartIndex = out_vertices.size();
        }
    }
    // last SubMesh added after reading all vertices
    if (out_vertices.size() > currentStartIndex) {
        SubMesh subset;
        subset.startIndex = currentStartIndex;
        subset.numVertices = out_vertices.size() - currentStartIndex;
        subset.material = materials[currentMaterialName];
        subMeshes.push_back(subset);
    }

    fclose(file);
    return true;
}

std::unordered_map<std::string, Material> ObjLoader::loadMTL(std::string& path) {

    std::unordered_map<std::string, Material> materials;
    FILE* file;
    if (fopen_s(&file, path.c_str(), "r") != 0 || file == nullptr) {
        std::cout << "Failed to open file: " << path << std::endl;
        return materials;
    }
    size_t slashIndex = path.find_last_of("/\\");
    std::string currentDir = path.substr(0, slashIndex + 1);
    char lineHeader[128];
    std::string currentMaterialName = "";
    // Read the first word of the line until the End Of File
    while (fscanf(file, "%127s", lineHeader) != EOF) {
        if (strcmp(lineHeader, "newmtl") == 0) {
            char matName[128];
            fscanf(file, "%127s\n", matName);
            currentMaterialName = matName;
            Material tempMat = Material();
            tempMat.name = currentMaterialName;
            materials[currentMaterialName] = tempMat;
        }
        else if (strcmp(lineHeader, "Ns") == 0) {
            float shininess;
            fscanf(file, "%f\n", &shininess);
            materials[currentMaterialName].shininess = shininess;
        }
        else if (strcmp(lineHeader, "Ka") == 0) {
            glm::vec3 ambientColor;
            fscanf(file, "%f %f %f\n", &ambientColor.x, &ambientColor.y, &ambientColor.z);
            materials[currentMaterialName].ambient = ambientColor;
        }
        else if (strcmp(lineHeader, "Kd") == 0) {
            glm::vec3 diffuseColor;
            fscanf(file, "%f %f %f\n", &diffuseColor.x, &diffuseColor.y, &diffuseColor.z);
            materials[currentMaterialName].diffuse = diffuseColor;
        }
        else if (strcmp(lineHeader, "Ks") == 0) {
            glm::vec3 specularColor;
            fscanf(file, "%f %f %f\n", &specularColor.x, &specularColor.y, &specularColor.z);
            materials[currentMaterialName].specular = specularColor;
        }
        else if (strcmp(lineHeader, "map_Kd") == 0) {
            char texPath[256];
            fscanf(file, "%255s\n", texPath);
            std::string fullPath = currentDir + std::string(texPath); // Or just use texPath if your .mtl paths are already perfectly relative to the project root!
            materials[currentMaterialName].diffuseTextureID = pgr::createTexture(fullPath);
        }
        else if (strcmp(lineHeader, "map_bump") == 0 || strcmp(lineHeader, "bump") == 0) {
            char texPath[256];
            fscanf(file, "%255s\n", texPath);
            std::string fullPath = currentDir + std::string(texPath);
            materials[currentMaterialName].normalTextureID = pgr::createTexture(fullPath);
        } 
        else if (strcmp(lineHeader, "map_Ks") == 0) {
            char texPath[256];
            fscanf(file, "%255s\n", texPath);
            printf("read: %s", texPath);
            std::string fullPath = currentDir + std::string(texPath);
            materials[currentMaterialName].specularTextureID = pgr::createTexture(fullPath);
        }
    }

    fclose(file);
    return materials;
}

bool ObjLoader::loadHardcode(const float* positions, const float* normals, const float* uvs,
                            const int vertex_count, const std::string& texturePath,
                            std::vector<glm::vec3>& out_vertices,
                            std::vector<glm::vec2>& out_uvs,
                            std::vector<glm::vec3>& out_normals,
                            std::vector<SubMesh>& subMeshes) {
    for (int i = 0; i < vertex_count; i++) {
        out_vertices.push_back(glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]));
        out_normals.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
        out_uvs.push_back(glm::vec2(uvs[i * 2], uvs[i * 2 + 1]));
    }
    SubMesh subMesh;
    subMesh.startIndex = 0;
    subMesh.numVertices = vertex_count;
    subMesh.material.ambient = glm::vec3(0.8f, 0.8f, 0.8f);
    subMesh.material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    subMesh.material.specular = glm::vec3(0.8f, 0.8f, 0.8f);
    subMesh.material.shininess = 0.5f;
    if (!texturePath.empty()) subMesh.material.diffuseTextureID = pgr::createTexture(texturePath);
    subMeshes.push_back(subMesh);
    return true;
}