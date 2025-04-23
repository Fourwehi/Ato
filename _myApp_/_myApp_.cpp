// ÅëÇÕ ¼öÁ¤µÈ _myApp_.cpp
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#define STB_IMAGE_IMPLEMENTATION
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"
#include <cmath>

class my_application : public sb7::application {
public:
        GLuint compile_shader1(void) {
        GLuint vertex_shader = sb7::shader::load("vs.glsl", GL_VERTEX_SHADER);
        GLuint fragment_shader = sb7::shader::load("fs.glsl", GL_FRAGMENT_SHADER);
        GLuint program1 = glCreateProgram();
        glAttachShader(program1, vertex_shader);
        glAttachShader(program1, fragment_shader);
        glLinkProgram(program1);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return program1;
    }

 void loadTexture(const char* filePath, GLuint& texID) {
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            int w, h, c;
            unsigned char* data = stbi_load(filePath, &w, &h, &c, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }


    virtual void startup() {
        stbi_set_flip_vertically_on_load(true);
        rendering_program1 = compile_shader1();

        GLfloat tentVertices[] = {
            // ²ÀÁþÁ¡ (À­ºÎºÐ)
            0.0f,  1.0f,  0.0f,    0.5f, 1.0f,  // 0: top

            // ¹Ù´Ú ²ÀÁþÁ¡ (x, y, z), (u, v)
           -1.0f, 0.0f,  1.0f,    0.0f, 1.0f,  // 1: front-left
            1.0f, 0.0f,  1.0f,    1.0f, 0.0f,  // 2: front-right
           -1.0f, 0.0f, -1.0f,    0.0f, 0.0f,  // 3: back-left
            1.0f, 0.0f, -1.0f,    1.0f, 0.0f   // 4: back-right
        };

        GLuint tentIndices[] = {
            0, 1, 2,  // ¾Õ¸é (CCW ¡æ Ä«¸Þ¶ó ÂÊ)
            0, 4, 3,  // µÞ¸é (CW ¡æ ¹Ý´ëÂÊ)
            0, 3, 1,  // ¿ÞÂÊ (CCW)
            0, 2, 4   // ¿À¸¥ÂÊ (CCW)
        };


        // ¹Ù´Ú
        GLfloat groundVertices[] = {
           -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,  // 0: back-left
            10.0f, 0.0f, -10.0f,   1.0f, 0.0f,  // 1: back-right
            10.0f, 0.0f,  10.0f,   1.0f, 1.0f,  // 2: front-right
           -10.0f, 0.0f,  10.0f,   0.0f, 1.0f   // 3: front-left
        };

        GLuint groundIndices[] = {
            0, 2, 1,  // CW
            0, 3, 2
        };

        //ÆøÆ÷
        GLfloat waterfallVertices[] = {
           -10.0f,  10.0f, 10.0f,   0.0f, 1.0f,  // 0: top-front
            -10.0f,  10.0f, -10.0f,   1.0f, 1.0f,  // 1: top-back
            -10.0f, 0.0f, 10.0f,   1.0f, 0.0f,  // 2: bottom-front
           -10.0f, 0.0f, -10.0f,   0.0f, 0.0f   // 3: bottom-back
        };
        GLuint waterfallIndices[] = {
            3, 1, 0,
            0, 2, 3  // CCW (Á¤¸é)
        };

        // ÅØ½ºÃ³ ·Îµù
        loadTexture("tent_side.jpg", tentSideTexture);
        loadTexture("tent_front.jpg", tentFrontTexture);
        loadTexture("tent_back.jpg", tentBackTexture);
        loadTexture("ground.jpg", groundTexture);
        loadTexture("waterfall.jpg", waterfallTexture);

        // ÅÙÆ® VAO/VBO/EBO
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

        // ¹Ù´Ú VAO
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

        // ÆøÆ÷ VAO
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
        

        glBindVertexArray(0);
    }

    virtual void render(double currentTime) {
        
        //const GLfloat black[] = { 0.98f, 0.67f, 0.3f, 1.0f };
        //glClearBufferfv(GL_COLOR, 0, black);
        glClearColor(0.98f, 0.67f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glUseProgram(rendering_program1);

		// Ä«¸Þ¶ó ¼³Á¤
        float camX = cos(currentTime) * 5.0f;
        float camZ = sin(currentTime) * 5.0f;
        vmath::vec3 eye(camX, 5.0f, camZ);
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::mat4 viewM = vmath::lookat(eye, target, up);
        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);

        // ¹Ù´Ú
        glBindVertexArray(groundVAO);
        glUniform1i(glGetUniformLocation(rendering_program1, "objectID"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(glGetUniformLocation(rendering_program1, "groundTexture"), 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //ÅÙÆ®
        glBindVertexArray(tentVAO);
        glUniform1i(glGetUniformLocation(rendering_program1, "objectID"), 0);
        glActiveTexture(GL_TEXTURE0);

        for (int i = 0; i < 3; ++i) {
            vmath::mat4 modelM = vmath::scale(tentScales[i]) * vmath::translate(tentPositions[i]);
            vmath::mat4 mvpM = projM * viewM * modelM;
            glUniformMatrix4fv(glGetUniformLocation(rendering_program1, "mvpM"), 1, GL_FALSE, mvpM);

            // ¾Õ¸é
            glUniform1i(glGetUniformLocation(rendering_program1, "faceID"), 0);
            glBindTexture(GL_TEXTURE_2D, tentFrontTexture);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));

            // µÞ¸é
            glUniform1i(glGetUniformLocation(rendering_program1, "faceID"), 1);
            glBindTexture(GL_TEXTURE_2D, tentBackTexture);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

            // ¿·¸é ¿ÞÂÊ
            glUniform1i(glGetUniformLocation(rendering_program1, "faceID"), 2);
            glBindTexture(GL_TEXTURE_2D, tentSideTexture);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

            // ¿·¸é ¿À¸¥ÂÊ
            glUniform1i(glGetUniformLocation(rendering_program1, "faceID"), 2);
            glBindTexture(GL_TEXTURE_2D, tentSideTexture);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(GLuint)));
        }
		glBindVertexArray(0);

        // ÆøÆ÷
        glBindVertexArray(waterfallVAO);
        float offset = fmod(currentTime, 1.0f);
        glUniform1f(glGetUniformLocation(rendering_program1, "textureOffset"), offset);
        glUniform1i(glGetUniformLocation(rendering_program1, "objectID"), 2);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterfallTexture);
        glUniform1i(glGetUniformLocation(rendering_program1, "waterfallTexture"), 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    virtual void shutdown() {
        glDeleteVertexArrays(1, &tentVAO);
        glDeleteBuffers(1, &tentVBO);
        glDeleteBuffers(1, &tentEBO);
        glDeleteVertexArrays(1, &groundVAO);
        glDeleteBuffers(1, &groundVBO);
        glDeleteBuffers(1, &groundEBO);
        glDeleteVertexArrays(1, &waterfallVAO);
        glDeleteBuffers(1, &waterfallVBO);
        glDeleteBuffers(1, &waterfallEBO);
        glDeleteProgram(rendering_program1);
    }

private:
    GLuint rendering_program1;
    GLuint tentVAO, tentVBO, tentEBO;
    GLuint groundVAO, groundVBO, groundEBO;
    GLuint waterfallVAO, waterfallVBO, waterfallEBO;

    GLuint tentSideTexture, tentFrontTexture, tentBackTexture;
    GLuint groundTexture, waterfallTexture;

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
	};
DECLARE_MAIN(my_application)

/*
        // MVP Çà·Ä °è»ê
        float angle = -currentTime;
        vmath::mat4 rm = vmath::rotate((angle * -100), 0.0f, 1.0f, 0.0f);
        vmath::mat4 tm = vmath::translate((float)sin(angle), 0.0f, (float)cos(angle));
        vmath::mat4 modelM = rm * tm;
        vmath::vec3 eye(0.0, 1.0, -2.0);
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::mat4 perspM = vmath::lookat(eye, target, up);
        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);
        vmath::mat4 mvpM = projM * perspM * modelM;

        // MVP À¯´ÏÆû ¼³Á¤
        GLint mvpMLocation = glGetUniformLocation(rendering_program1, "mvpM");
        glUniformMatrix4fv(mvpMLocation, 1, GL_FALSE, mvpM);
        */