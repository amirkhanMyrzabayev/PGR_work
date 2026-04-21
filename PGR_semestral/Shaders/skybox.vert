#version 140
in vec2 screenCoord;
out vec2 texCoord;



void main() {
  texCoord = screenCoord;
  gl_Position = vec4(screenCoord, 1.0, 1.0);
}
