#version 140


in vec3 vertexPosition;
in vec3 normalVector;
in vec2 textureCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;
uniform sampler2D diffuseMap;
uniform int hasDiffuseMap;

out vec4 color;

void main() {
  vec3 norm = normalize(normalVector);
  vec3 baseDiffuse;
  if (hasDiffuseMap == 1) {
    baseDiffuse = texture(diffuseMap, textureCoord).rgb;
  } else {
    baseDiffuse = matDiffuse;
  }
  vec3 lightDir = normalize(lightPos - vertexPosition);
  vec3 viewDir = normalize(viewPos - vertexPosition);
  float ambientIntensity = 0.1;
  float diffuse = max(dot(lightDir, norm), 0);
  float specular = pow(max(dot(viewDir, reflect(-lightDir, norm)), 0.0), matShininess);
  vec3 finalColor = (ambientIntensity * baseDiffuse) + (diffuse * baseDiffuse) + (specular * matSpecular);
  color = vec4(finalColor, 1.0);
}