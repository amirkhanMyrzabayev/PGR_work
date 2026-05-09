#version 140
in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 texMatrix;
uniform float elapsedTime;

in vec3 normal;
in vec2 texCoord;

out vec3 vertexPosition;
out vec3 normalVector;
out vec2 textureCoord;

const float frequency = 10.0;
const float speed = 5.0;
const float threshold = 0.9;
const float moveOffset = 0.8;

void main() {
    vec3 localPos = position;

    float waveValue = sin(localPos.y * frequency + elapsedTime * speed);    
    if (waveValue > threshold) {
        localPos.x += moveOffset; 
    }
    vertexPosition = vec3(model * vec4(localPos, 1.0));
    gl_Position = projection * view * model * vec4(localPos, 1.0);
    normalVector = normalize(normalMatrix * normal);
    textureCoord = (texMatrix * vec4(texCoord, 0.0, 1.0)).xy;

}
