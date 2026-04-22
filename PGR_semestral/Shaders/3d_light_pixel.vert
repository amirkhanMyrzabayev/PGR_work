#version 140
in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

in vec3 normal;
in vec2 texCoord;

out vec3 vertexPosition;
out vec3 normalVector;
out vec2 textureCoord;

void main() {
  vertexPosition =  vec3(model * vec4(position, 1.0));
  normalVector = normalize(normalMatrix * normal);
  textureCoord = texCoord;
  gl_Position = projection * view * model * vec4(position, 1.0f);
  
}
