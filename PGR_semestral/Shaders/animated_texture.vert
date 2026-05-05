#version 140
in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec2 normal;
in vec2 texCoord;

uniform int columns;
uniform int rows;
uniform int currentFrame;

out vec2 textureCoord;
out vec3 vertexPosition;


void main() {
  float column = float(currentFrame % columns);
  float row = float(currentFrame / columns);
  row = float(rows - 1) - row;
  float offsetX = column / float(columns);
  float offsetY = row / float(rows);
  vec2 scaledUV = vec2(texCoord.x / float(columns), texCoord.y / float(rows));
  textureCoord = scaledUV + vec2(offsetX, offsetY);
  vertexPosition =  vec3(model * vec4(position, 1.0));
  gl_Position = projection * view * vec4(vertexPosition, 1.0);
}
