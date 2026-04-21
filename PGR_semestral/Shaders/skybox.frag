#version 140

in vec2 texCoord;
out vec4 color;
uniform samplerCube skyboxCube;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 coord = vec4(texCoord, 1.0, 1.0);
    coord = projection*coord;
    coord = coord/coord.w;
    coord.w = 0.0;
    vec4 worldDir = view * coord;
    color = texture(skyboxCube, worldDir.xyz);
}