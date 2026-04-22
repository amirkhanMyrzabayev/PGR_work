#version 140
in vec3 vertexColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

out vec4 color;

void main() {
  color = vec4(vertexColor, 1.0f);
}