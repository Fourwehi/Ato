#version 430 core

in vec3 vsPos;
in vec3 vsNormal;
in vec2 vsTexCoord;

// 머티리얼 구조체: 디퓨즈 맵, 스페큘러 맵, shininess
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

// 라이트 구조체: 위치 + ambient/diffuse/specular 강도
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform vec3 viewPos;  // 카메라 위치

out vec4 fragColor;

void main()
{
    // 1) Ambient
    vec3 texColor  = vec3(texture(material.diffuse, vsTexCoord));
    vec3 ambient   = light.ambient * texColor;

    // 2) Diffuse
    vec3 norm     = normalize(vsNormal);
    vec3 lightDir = normalize(light.position - vsPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * texColor);

    // 3) Specular
    vec3 viewDir    = normalize(viewPos - vsPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specMap    = vec3(texture(material.specular, vsTexCoord));
    vec3 specular   = light.specular * spec * specMap;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
