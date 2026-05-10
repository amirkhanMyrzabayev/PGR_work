#version 140


in vec2 textureCoord;
out vec4 color;

uniform sampler2D diffuseMap;
uniform float time;      
uniform float slowSpeed;

void main() {
    float angle = time * slowSpeed; 
    // Shift UV to center (0,0), rotate it, then shift it back
    vec2 shiftedUV = textureCoord - vec2(0.5, 0.5);
    
    mat2 rotationMatrix = mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );
    vec2 rotatedUV = rotationMatrix * shiftedUV;
    vec2 finalUV = rotatedUV + vec2(0.5, 0.5);
    // Discard pixels that rotate outside the 0.0 - 1.0 UV bounds
    if(finalUV.x < 0.0 || finalUV.x > 1.0 || finalUV.y < 0.0 || finalUV.y > 1.0) {
        color = vec4(0.0);
    } else {
        color = texture(diffuseMap, finalUV);
    }
    if (color.a <= 0.1) discard;
}