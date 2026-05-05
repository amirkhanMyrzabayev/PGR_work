#version 140
in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 texMatrix;

in vec3 normal;
in vec2 texCoord;

out vec3 vertexPosition;
out vec3 normalVector;
out vec2 textureCoord;

void main() {
  vertexPosition = vec3(model * vec4(position, 1.0));
  normalVector = normalize(normalMatrix * normal);
  textureCoord = (texMatrix * vec4(texCoord, 0.0, 1.0)).xy;
  gl_Position = projection * view * model * vec4(position, 1.0);
  
}
