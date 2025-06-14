#version 330

uniform sampler2D textureMap0;
uniform int mode; // 0: kolor, 1: tekstura

in vec4 vColor;
in vec2 iTexCoord0;
in vec4 vNormal;
in vec4 vLightDir;
in vec4 vViewDir;

in vec4 vNormal2;
in vec4 vLightDir2;
in vec4 vViewDir2;

out vec4 pixelColor;

void main(void) {
    if (mode == 0) {
        pixelColor = vColor;  // tylko kolor
    } else if (mode == 1) {
        vec4 texColor = texture(textureMap0, iTexCoord0);
        vec4 kd = texColor; //kolor dyfuzyjny
        vec4 ks = vec4(1.0); // bialy specular
        
        // swiatlo 1
        vec4 N = normalize(vNormal);
        vec4 L = normalize(vLightDir);
        vec4 V = normalize(vViewDir);
        vec4 R = reflect(-L, N);

        float diffuse = max(dot(N, L), 0.0);
        float specular = pow(max(dot(R, V), 0.0), 50.0);

        // swiatlo 2
        vec4 N2 = normalize(vNormal2);
        vec4 L2 = normalize(vLightDir2);
        vec4 V2 = normalize(vViewDir2);
        vec4 R2 = reflect(-L2, N2);

        float diffuse2 = max(dot(N2, L2), 0.0);
        float specular2 = pow(max(dot(R2, V2), 0.0), 50.0);

        pixelColor = vec4(kd.rgb * (diffuse + diffuse2) + ks.rgb * (specular + specular2) , 1.0);
    }
}