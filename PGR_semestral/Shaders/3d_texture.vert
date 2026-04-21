#version 140
in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

in vec3 normal;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;


out vec3 vertexColor;

void main() {
  vec3 worldPos =  vec3(model * vec4(position, 1.0));
  vec3 worldNormal = normalize(normalMatrix * normal);
  vec3 lightDir =  normalize(lightPos - worldPos);
  float ambientIntensity = 0.1f;
  float diffuseIntensity = max(dot(worldNormal, lightDir), 0.0f);
  float specularIntensity = pow(max(dot(normalize(viewPos - worldPos), reflect(-lightDir, worldNormal)), 0.0f), matShininess);
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vertexColor = (ambientIntensity * matAmbient + 
                 specularIntensity * matSpecular + 
                 diffuseIntensity * matDiffuse);
}
