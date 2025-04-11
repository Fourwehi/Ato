#version 430 core                                                   
                                                                          
 layout(location = 3) in float time;                              
     
     
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
    float far =  10.0;

    mat4 viewMatrix = lookAt(eye, target, up);
    mat4 projectionMatrix = perspective(left, right, top, bottom, near, far);

void main(void)                                                      
{                                                                         
   const vec4 vertices[6] = vec4[6](vec4(0.01, 0.0, 0.5, 1.0),
                                   vec4(0.01, -1.0, 0.5, 1.0),        
                                   vec4(-0.01, -1.0, 0.5, 1.0),       

                                   vec4(-0.01, 0.0, 0.5, 1.0),        
                                   vec4(-0.01, -1.0, 0.5, 1.0),       
                                   vec4(0.01, 0.0, 0.5, 1.0));         

 mat4 tM = mat4(1.0);                                             
 tM[3][0] = sin(time * 0.5) * 0.5;                                 
gl_Position = projectionMatrix * viewMatrix * tM * vertices[gl_VertexID];
}                                                                          