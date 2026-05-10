#version 140
uniform mat4 model;
in vec2 position;
in vec2 texCoord;

out vec2 textureCoord;

void main() {
    textureCoord = texCoord;
    gl_Position = model * vec4(position, 0.0, 1.0);
}