#pragma once
#include "pgr.h"

/// @brief Stores lighting colors and texture IDs for rendering.
struct Material {
    glm::vec3 ambient;            ///< Ambient base color.
    glm::vec3 diffuse;            ///< Main object color under direct light.
    glm::vec3 specular;           ///< Color of the light reflection.
    std::string name;
    float shininess;              ///< How shiny/glossy the surface is.
    float alpha = 1.0f;           ///< Transparency (1.0 is fully solid, 0.0 is invisible).
    GLuint diffuseTextureID = 0;  ///< OpenGL ID for the main color image.
    GLuint normalTextureID = 0;   ///< OpenGL ID for the bump/normal map.
    GLuint specularTextureID = 0; ///< OpenGL ID for the reflection map.
};

/// @brief A piece of a 3D model that shares the same material.
struct SubMesh {
    unsigned int startIndex;      ///< Index of the first vertex to draw in the buffer.
    unsigned int numVertices;     ///< Total number of vertices in this piece.
    Material material;
};

/// @brief Main structure holding OpenGL buffers and model parts.
struct Mesh {
    GLuint vao;                     ///< Vertex Array Object ID.
    GLuint vbo;                     ///< Vertex Buffer Object ID.
    std::vector<SubMesh> subMeshes; ///< All the individual parts of the model.
};

/// @brief Data layout for a single point in 3D space.
struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;    ///< Direction the vertex faces (used for lighting).
    glm::vec2 Texcoord;  ///< 2D coordinates for wrapping textures (U, V).
};