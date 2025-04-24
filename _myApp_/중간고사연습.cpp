/*
#define STB_IMAGE_IMPLEMENTATION
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"
#include <vector>

class my_application : public sb7::application {
public:
    GLuint tentVAO, tentVBO, tentEBO;
    GLuint groundVAO, groundVBO, groundEBO;
    GLuint waterfallVAO, waterfallVBO, waterfallEBO;
    GLuint rendering_program;
    GLuint tentTextures[3];
    GLuint groundTexture;
    GLuint waterfallTexture;

    vmath::vec3 tentPositions[3] = {
       vmath::vec3(-2.0f, 0.0f, -3.0f),
       vmath::vec3(3.0f, 0.0f, -3.0f),
       vmath::vec3(0.0f, 0.0f, 1.0f)
    };
    vmath::vec3 tentScales[3] = {
        vmath::vec3(1.0f, 1.0f, 1.0f),
        vmath::vec3(1.2f, 1.2f, 1.2f),
        vmath::vec3(0.8f, 0.8f, 0.8f)
    };

    GLuint loadTexture(const char* filepath) {
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format;
            if (nrChannels == 4) {
                format = GL_RGBA;
            }
            else {
                format = GL_RGB;
            }glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return tex;
    }

    void createTent() {
        GLfloat tentVertices[] = {
            // 꼭짓점 (윗부분)
            0.0f,  1.0f,  0.0f,    0.5f, 1.0f,  // 0: top

            // 바닥 꼭짓점 (x, y, z), (u, v)
           -1.0f, 0.0f,  1.0f,    0.0f, 1.0f,  // 1: front-left
            1.0f, 0.0f,  1.0f,    1.0f, 0.0f,  // 2: front-right
           -1.0f, 0.0f, -1.0f,    0.0f, 0.0f,  // 3: back-left
            1.0f, 0.0f, -1.0f,    1.0f, 0.0f   // 4: back-right
        };

        GLuint tentIndices[] = {
            0, 1, 2,  // 앞면 (CCW → 카메라 쪽)
            0, 4, 3,  // 뒷면 (CW → 반대쪽)
            0, 3, 1,  // 왼쪽 (CCW)
            0, 2, 4   // 오른쪽 (CCW)
        };

        glGenVertexArrays(1, &tentVAO);
        glGenBuffers(1, &tentVBO);
        glGenBuffers(1, &tentEBO);
        glBindVertexArray(tentVAO);
        glBindBuffer(GL_ARRAY_BUFFER, tentVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tentVertices), tentVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tentEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tentIndices), tentIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createGround() {

        // 바닥
        GLfloat groundVertices[] = {
           -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,  // 0: back-left
            10.0f, 0.0f, -10.0f,   1.0f, 0.0f,  // 1: back-right
            10.0f, 0.0f,  10.0f,   1.0f, 1.0f,  // 2: front-right
           -10.0f, 0.0f,  10.0f,   0.0f, 1.0f   // 3: front-left
        };

        GLuint groundIndices[] = {
            0, 2, 1,
            0, 3, 2
        };
        glGenVertexArrays(1, &groundVAO);
        glGenBuffers(1, &groundVBO);
        glGenBuffers(1, &groundEBO);
        glBindVertexArray(groundVAO);
        glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createWaterfall() {

        GLfloat waterfallVertices[] = {
          -10.0f,  10.0f, 10.0f,   0.0f, 1.0f,  // 0: top-front
           -10.0f,  10.0f, -10.0f,   1.0f, 1.0f,  // 1: top-back
           -10.0f, 0.0f, 10.0f,   1.0f, 0.0f,  // 2: bottom-front
          -10.0f, 0.0f, -10.0f,   0.0f, 0.0f   // 3: bottom-back
        };
        GLuint waterfallIndices[] = {
            3, 1, 0,
            0, 2, 3 
        };
        glGenVertexArrays(1, &waterfallVAO);
        glGenBuffers(1, &waterfallVBO);
        glGenBuffers(1, &waterfallEBO);
        glBindVertexArray(waterfallVAO);
        glBindBuffer(GL_ARRAY_BUFFER, waterfallVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waterfallVertices), waterfallVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterfallEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(waterfallIndices), waterfallIndices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    GLuint compile_shader1(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint rendering_program;

        // 버텍스 쉐이더를 생성하고 컴파일한다.
        vertex_shader = sb7::shader::load("vs.glsl", GL_VERTEX_SHADER);

        // 프래그먼트 쉐이더를 생성하고 컴파일한다.
        fragment_shader = sb7::shader::load("fs.glsl", GL_FRAGMENT_SHADER);

        //프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
        rendering_program = glCreateProgram();
        glAttachShader(rendering_program, vertex_shader);
        glAttachShader(rendering_program, fragment_shader);
        glLinkProgram(rendering_program);

        //이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return rendering_program;

    }

    virtual void startup() {
        stbi_set_flip_vertically_on_load(true);
		rendering_program = compile_shader1();

        createTent();
        createGround();
        createWaterfall();

        tentTextures[0] = loadTexture("tent_front.jpg");
        tentTextures[1] = loadTexture("tent_side.jpg");
        tentTextures[2] = loadTexture("tent_back.jpg");
        groundTexture = loadTexture("ground.jpg");
        waterfallTexture = loadTexture("waterfall.jpg");
    }

    virtual void render(double currentTime) {
        glClearColor(0.98f, 0.67f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glUseProgram(rendering_program);

        float camX = cos(currentTime) * 5.0f;
        float camZ = sin(currentTime) * 5.0f;
        vmath::vec3 eye(camX, 8.0f, camZ);
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::mat4 view = vmath::lookat(eye, target, up);
        vmath::mat4 proj = vmath::perspective(45.0f, float(info.windowWidth) / info.windowHeight, 0.1f, 100.0f);
        vmath::mat4 vp = proj * view;

		//Ground
        glBindVertexArray(groundVAO);
        glUniform1i(glGetUniformLocation(rendering_program, "objectID"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(glGetUniformLocation(rendering_program, "groundTexture"), 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Waterfall
        glBindVertexArray(waterfallVAO);
        float offset = fmod(currentTime, 1.0f);
        glUniform1f(glGetUniformLocation(rendering_program, "textureOffset"), offset);
        glUniform1i(glGetUniformLocation(rendering_program, "objectID"), 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterfallTexture);
        glUniform1i(glGetUniformLocation(rendering_program, "waterfallTexture"), 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //텐트
        glBindVertexArray(tentVAO);
        glUniform1i(glGetUniformLocation(rendering_program, "objectID"), 0);
        glActiveTexture(GL_TEXTURE0);
        for (int i = 0; i < 3; ++i) {
            vmath::mat4 modelM = vmath::scale(tentScales[i]) * vmath::translate(tentPositions[i]);
            vmath::mat4 mvpM = proj * view * modelM;
            glUniformMatrix4fv(glGetUniformLocation(rendering_program, "mvpM"), 1, GL_FALSE, mvpM);

            // 앞면
            glUniform1i(glGetUniformLocation(rendering_program, "faceID"), 0);
            glBindTexture(GL_TEXTURE_2D, tentTextures[0]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));

            // 뒷면
            glUniform1i(glGetUniformLocation(rendering_program, "faceID"), 1);
            glBindTexture(GL_TEXTURE_2D, tentTextures[1]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

            // 옆면 왼쪽
            glUniform1i(glGetUniformLocation(rendering_program, "faceID"), 2);
            glBindTexture(GL_TEXTURE_2D, tentTextures[1]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

            // 옆면 오른쪽
            glUniform1i(glGetUniformLocation(rendering_program, "faceID"), 2);
            glBindTexture(GL_TEXTURE_2D, tentTextures[2]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(GLuint)));
        }
        glBindVertexArray(0);
    }

    virtual void shutdown() {
        glDeleteVertexArrays(1, &tentVAO);
        glDeleteVertexArrays(1, &groundVAO);
        glDeleteVertexArrays(1, &waterfallVAO);
        glDeleteBuffers(1, &tentVBO);
        glDeleteBuffers(1, &groundVBO);
        glDeleteBuffers(1, &waterfallVBO);
        glDeleteProgram(rendering_program);
    }
};

DECLARE_MAIN(my_application)
*/