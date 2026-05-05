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
            char tempLine[256];
            fgets(tempLine, 256, file);
            int offset = 0;
            std::vector<unsigned int> vIndices;
            std::vector<unsigned int> uvIndices;
            std::vector<unsigned int> nIndices;


            int vertexIndex, uvIndex, normalIndex;
            int bytesRead;
            while (sscanf(tempLine + offset, "%d/%d/%d%n", &vertexIndex, &uvIndex, &normalIndex, &bytesRead) == 3) {
                vIndices.push_back(vertexIndex-1);
                uvIndices.push_back(uvIndex-1);
                nIndices.push_back(normalIndex-1);
                offset += bytesRead;
            }
            if (vIndices.size() == 3) {
                out_vertices.push_back(temp_vertices[vIndices[0]]);
                out_vertices.push_back(temp_vertices[vIndices[1]]);
                out_vertices.push_back(temp_vertices[vIndices[2]]);
                out_uvs.push_back(temp_uvs[uvIndices[0]]);
                out_uvs.push_back(temp_uvs[uvIndices[1]]);
                out_uvs.push_back(temp_uvs[uvIndices[2]]);
                out_normals.push_back(temp_normals[nIndices[0]]);
                out_normals.push_back(temp_normals[nIndices[1]]);
                out_normals.push_back(temp_normals[nIndices[2]]);
            }
            else if (vIndices.size() == 4) {
                out_vertices.push_back(temp_vertices[vIndices[0]]);
                out_vertices.push_back(temp_vertices[vIndices[1]]);
                out_vertices.push_back(temp_vertices[vIndices[2]]);
                out_uvs.push_back(temp_uvs[uvIndices[0]]);
                out_uvs.push_back(temp_uvs[uvIndices[1]]);
                out_uvs.push_back(temp_uvs[uvIndices[2]]);
                out_normals.push_back(temp_normals[nIndices[0]]);
                out_normals.push_back(temp_normals[nIndices[1]]);
                out_normals.push_back(temp_normals[nIndices[2]]);
                out_vertices.push_back(temp_vertices[vIndices[0]]);
                out_vertices.push_back(temp_vertices[vIndices[2]]);
                out_vertices.push_back(temp_vertices[vIndices[3]]);
                out_uvs.push_back(temp_uvs[uvIndices[0]]);
                out_uvs.push_back(temp_uvs[uvIndices[2]]);
                out_uvs.push_back(temp_uvs[uvIndices[3]]);
                out_normals.push_back(temp_normals[nIndices[0]]);
                out_normals.push_back(temp_normals[nIndices[2]]);
                out_normals.push_back(temp_normals[nIndices[3]]);
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
    std::cout << "OBJ loaded: " << path << std::endl;
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
        if (_stricmp(lineHeader, "newmtl") == 0) {
            char matName[128];
            fscanf(file, "%127s\n", matName);
            currentMaterialName = matName;
            Material tempMat = Material();
            tempMat.name = currentMaterialName;
            materials[currentMaterialName] = tempMat;
        }
        else if (_stricmp(lineHeader, "Ns") == 0) {
            float shininess;
            fscanf(file, "%f\n", &shininess);
            materials[currentMaterialName].shininess = shininess;
        }
        else if (_stricmp(lineHeader, "Ka") == 0) {
            glm::vec3 ambientColor;
            fscanf(file, "%f %f %f\n", &ambientColor.x, &ambientColor.y, &ambientColor.z);
            materials[currentMaterialName].ambient = ambientColor;
        }
        else if (_stricmp(lineHeader, "Kd") == 0) {
            glm::vec3 diffuseColor;
            fscanf(file, "%f %f %f\n", &diffuseColor.x, &diffuseColor.y, &diffuseColor.z);
            materials[currentMaterialName].diffuse = diffuseColor;
        }
        else if (_stricmp(lineHeader, "Ks") == 0) {
            glm::vec3 specularColor;
            fscanf(file, "%f %f %f\n", &specularColor.x, &specularColor.y, &specularColor.z);
            materials[currentMaterialName].specular = specularColor;
        }
        else if (_stricmp(lineHeader, "map_Kd") == 0) {
            char texPath[256];
            fscanf(file, "%255s\n", texPath);
            std::string fullPath = currentDir + std::string(texPath);

            materials[currentMaterialName].diffuseTextureID = pgr::createTexture(fullPath);
            std::cout << "tried to load: " << texPath << " Id: " << materials[currentMaterialName].diffuseTextureID << std::endl;
        }
        else if (_stricmp(lineHeader, "map_bump") == 0 || _stricmp(lineHeader, "bump") == 0) {
            char texPath[256];
            fscanf(file, "%255s\n", texPath);
            std::string fullPath = currentDir + std::string(texPath);
            
            materials[currentMaterialName].normalTextureID = pgr::createTexture(fullPath);
            std::cout << "tried to load: " << texPath << std::endl;
        } 
        else if (_stricmp(lineHeader, "map_Ks") == 0) {
            char texPath[256];
            fscanf(file, "%255s\n", texPath);
            //printf("read: %s", texPath);
            std::string fullPath = currentDir + std::string(texPath);
            materials[currentMaterialName].specularTextureID = pgr::createTexture(fullPath);
            std::cout << "tried to load: " << texPath << std::endl;
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
    if (!texturePath.empty()) {
        subMesh.material.diffuseTextureID = pgr::createTexture(texturePath);
        std::cout << "tried to load: " << texturePath << " Id: " << subMesh.material.diffuseTextureID << std::endl;
    }
    subMeshes.push_back(subMesh);
    return true;
}