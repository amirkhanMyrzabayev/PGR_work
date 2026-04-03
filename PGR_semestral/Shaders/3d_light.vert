#version 140
in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

in vec3 normal;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 vertexColor;

void main() {
  vec3 worldPos =  vec3(model * vec4(position, 1.0));
  vec3 worldNormal = normalize(normalMatrix * normal);
  vec3 lightDir =  normalize(lightPos - worldPos);
  float ambient = 0.1f;
  float diffuse = max(dot(worldNormal, lightDir), 0.0f);
  float specular = pow(max(dot(normalize(viewPos - worldPos), reflect(-lightDir, worldNormal)), 0.0f), 32);
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vertexColor = (ambient + specular + diffuse) * vec3(0.5f, 0.5f, 0.5f);
}
