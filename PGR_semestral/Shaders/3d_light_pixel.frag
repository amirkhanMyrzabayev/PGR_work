#version 140

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float linear;
    float quadratic;
    float constant;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
    vec3 position;

    float innerCutOff;
    float outerCutOff;

    float linear;
    float quadratic;
    float constant;
};

in vec3 vertexPosition;
in vec3 normalVector;
in vec2 textureCoord;

uniform vec3 viewPos;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;
uniform sampler2D diffuseMap;
uniform int hasDiffuseMap;
uniform sampler2D specularMap;
uniform int hasSpecularMap;

uniform DirLight dirLights[5];
uniform int numDirLights;

uniform PointLight pointLights[5];
uniform int numPointLights;

uniform SpotLight spotLights[5];
uniform int numSpotLights;


out vec4 color;

vec3 calcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir, 
                    vec3 baseDiffuse, vec3 baseSpecular)  {
    vec3 finalColor = vec3(0.0f);
    vec3 lightDir = normalize(-dirLight.direction);
    float diffuse = max(dot(lightDir, normal), 0);
    float specular = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), matShininess);
    vec3 ambientColor = dirLight.ambient * baseDiffuse * matAmbient;
    vec3 diffuseColor = dirLight.diffuse * diffuse * baseDiffuse;
    vec3 specularColor = dirLight.specular * specular * baseSpecular;
    finalColor += ambientColor + diffuseColor + specularColor;
    return finalColor;
}

vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir,  
                    vec3 baseDiffuse, vec3 baseSpecular, vec3 fragPos) {
    vec3 finalColor = vec3(0.0f);
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diffuse = max(dot(lightDir, normal), 0);
    float specular = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), matShininess);
    float dist = distance(pointLight.position, fragPos);
    float decay = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * (dist * dist));
    vec3 ambientColor = pointLight.ambient * baseDiffuse * matAmbient;
    vec3 diffuseColor = pointLight.diffuse * diffuse * baseDiffuse;
    vec3 specularColor = pointLight.specular * specular * baseSpecular;
    finalColor += ambientColor + diffuseColor + specularColor;
    finalColor *= decay;
    return finalColor;
}

vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir,  
                    vec3 baseDiffuse, vec3 baseSpecular, vec3 fragPos) {
    vec3 finalColor = vec3(0.0f);
    vec3 lightDir = normalize(spotLight.position - fragPos);
    float diffuse = max(dot(lightDir, normal), 0);
    float specular = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), matShininess);
    float dist = distance(spotLight.position, fragPos);
    float decay = 1.0 / (spotLight.constant + spotLight.linear * dist + spotLight.quadratic * (dist * dist));
    float coneAngle = dot(lightDir, normalize(-spotLight.direction));
    float intensity = smoothstep(spotLight.outerCutOff, spotLight.innerCutOff, coneAngle);
    vec3 ambientColor = spotLight.ambient * baseDiffuse * matAmbient;
    vec3 diffuseColor = spotLight.diffuse * diffuse * baseDiffuse * intensity;
    vec3 specularColor = spotLight.specular * specular * baseSpecular * intensity;
    finalColor += ambientColor + diffuseColor + specularColor;
    finalColor *= decay;
    return finalColor;
}

void main() {
  vec3 norm = normalize(normalVector);
  vec3 baseDiffuse;
  if (hasDiffuseMap == 1) {
    baseDiffuse = texture(diffuseMap, textureCoord).rgb;
  } else {
    baseDiffuse = matDiffuse;
  }
  vec3 baseSpecular;
  if (hasSpecularMap == 1) {
    baseSpecular = texture(specularMap, textureCoord).rgb;
  } else {
    baseSpecular = matSpecular;
  }
  
  vec3 viewDir = normalize(viewPos - vertexPosition);
  vec3 finalColor = vec3(0.0);

  for (int i = 0; i < numDirLights; i++) {
    finalColor += calcDirLight(dirLights[i], norm, viewDir, baseDiffuse, baseSpecular);
  }

  for (int i = 0; i < numPointLights; i++) {
    finalColor += calcPointLight(pointLights[i], norm, viewDir, baseDiffuse, baseSpecular, vertexPosition);
  }

  for (int i = 0; i < numSpotLights; i++) {
    finalColor += calcSpotLight(spotLights[i], norm, viewDir, baseDiffuse, baseSpecular, vertexPosition);
  }

  color = vec4(finalColor, 1.0);
}