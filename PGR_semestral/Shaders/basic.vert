#version 140
in vec2 position;
uniform mat4 view;
uniform mat4 projection;
void main() {
  gl_Position = projection * view * vec4(position, 0.0f, 1.0f);
}