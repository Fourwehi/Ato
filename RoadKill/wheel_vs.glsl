#version 430 core

layout (location = 0) in vec4 colors0;
layout (location = 1) in vec4 colors1;
layout (location = 2) in vec4 colors2;

layout (location = 3) in float time;
out vec4 vs_color;

mat4 lookAt(vec3 eye, vec3 target, vec3 up) {
    vec3 zAxis = normalize(eye - target); // 카메라 방향
    vec3 xAxis = normalize(cross(up, zAxis)); // 오른쪽 방향
    vec3 yAxis = cross(zAxis, xAxis); // 위쪽 방향

    mat4 rotation = transpose( mat4(
        vec4(xAxis, 0.0),
        vec4(yAxis, 0.0),
        vec4(zAxis, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    ) );

    mat4 translation = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(-eye, 1.0)
    );

    return rotation * translation;
}

mat4 perspective(float left, float right, float top, float bottom, float near, float far) {

    mat4 result = mat4(
        vec4((2.0*near)/(right-left), 0.0, 0.0, 0.0),
        vec4(0.0, (2.0*near)/(top-bottom), 0.0, 0.0),
        vec4((right+left)/(right-left),(top+bottom)/(top-bottom),(near+far)/(near-far), -1.0),
        vec4(0.0, 0.0, (2.0*near*far)/(near-far), 0.0)
    );

    return result;
}

    vec3 eye = vec3(0.2, 0.3, 0.8);
    vec3 target = vec3(0.0, 0.0, 0.5);
    vec3 up = vec3(0.0, 1.0, 0.0);

    float left = -0.1;
    float right = 0.1;
    float top = 0.07;
    float bottom = -0.07;
    float near = 0.1;
    float far =  1.0;

    mat4 viewMatrix = lookAt(eye, target, up);
    mat4 projectionMatrix = perspective(left, right, top, bottom, near, far);

void main(void) {
    const vec4 vertices[12] = vec4[12](
        vec4(0.0, 0.0, 0.5, 1.0),
        vec4(-0.5, 0.5, 0.5, 1.0),
        vec4(0.0, 0.5, 0.5, 1.0),
        vec4(0.0, 0.0, 0.5, 1.0),
        vec4(-0.5, -0.5, 0.5, 1.0),
        vec4(-0.5, 0.0, 0.5, 1.0),
        vec4(0.0, 0.0, 0.5, 1.0),
        vec4(0.5, -0.5, 0.5, 1.0),
        vec4(0.0, -0.5, 0.5, 1.0),
        vec4(0.0, 0.0, 0.5, 1.0),
        vec4(0.5, 0.5, 0.5, 1.0),
        vec4(0.5, 0.0, 0.5, 1.0)
    );

    const vec4 colors[3] = vec4[3](colors0, colors1, colors2);

    float angle = time * 150.0;
    mat4 rM = mat4(1.0);
    rM[0][0] = cos(radians(angle));
    rM[1][0] = -sin(radians(angle));
    rM[0][1] = sin(radians(angle));
    rM[1][1] = cos(radians(angle));
    mat4 tM = mat4(1.0);
    tM[3][0] = sin(time * 0.5) * 0.5;



    gl_Position = projectionMatrix * viewMatrix * tM * rM * vertices[gl_VertexID];
    vs_color = colors[gl_VertexID % 3];
}