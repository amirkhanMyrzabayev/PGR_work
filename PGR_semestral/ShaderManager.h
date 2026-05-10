#pragma once
#include <unordered_map>
#include <string>
#include "pgr.h"

/// @brief Manager class to load, store, and reuse OpenGL shader programs.
class ShaderManager
{
public:
    ~ShaderManager();

    /// @brief Gets a shader program by name. If it is not loaded yet, it reads it from .vert and .frag files.
    /// @param shaderName Base name of the shader files (without the .vert or .frag extension).
    /// @return The OpenGL ID of the ready-to-use shader program.
    GLuint getShaderProgram(const std::string& shaderName);

    /// @brief Sends fog settings to all currently loaded shaders that have fog variables.
    /// @param fogColor The RGB color of the fog.
    /// @param fogStart Distance from the camera where the fog starts to become visible.
    /// @param fogEnd Distance from the camera where you can't see anything through the fog.
    void setFogInShaders(glm::vec3 fogColor, float fogStart, float fogEnd);

private:
    /// Map keeping track of loaded shaders so we don't load them twice (Key: name, Value: OpenGL ID).
    std::unordered_map<std::string, GLuint> programs;
};