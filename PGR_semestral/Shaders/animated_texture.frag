#version 140
out vec4 color;
in vec2 textureCoord;
in vec3 vertexPosition;
uniform sampler2D diffuseMap;

uniform vec3 viewPos;

uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

void main() {
  vec4 texColor = texture(diffuseMap, textureCoord);
  if (texColor.a < 0.1) discard; 
  vec3 finalColor = texColor.rgb;
  float fogDist = fogEnd - distance(viewPos, vertexPosition);
  float visibilityFactor =  clamp(fogDist/(fogEnd - fogStart), 0.0, 1.0);
  finalColor = mix(fogColor, finalColor, visibilityFactor);
  color = vec4(finalColor, texColor.a);
}