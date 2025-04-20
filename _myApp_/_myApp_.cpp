#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#define STB_IMAGE_IMPLEMENTATION

// sb7.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"

// sb7::application�� ��ӹ޴´�.
class my_application : public sb7::application
{
public:
    // ���̴� �������Ѵ�.
    GLuint compile_shader1(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program1;

        // ���ؽ� ���̴��� �����ϰ� �������Ѵ�.
        vertex_shader = sb7::shader::load("box_vs.glsl", GL_VERTEX_SHADER);

        // �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
        fragment_shader = sb7::shader::load("box_fs.glsl", GL_FRAGMENT_SHADER);

        //���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
        program1 = glCreateProgram();
        glAttachShader(program1, vertex_shader);
        glAttachShader(program1, fragment_shader);
        glLinkProgram(program1);

        //���� ���α׷��� ���̴��� �����ϹǷ� ���̴��� �����Ѵ�.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program1;

    }


    //���ø����̼� �ʱ�ȭ �����Ѵ�.
    virtual void startup() {
        stbi_set_flip_vertically_on_load(true);

        rendering_program1 = compile_shader1();

        GLfloat vertices[] = {

            //�ո�
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
           -0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
           -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3  


            //�����ʸ�
            0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3 

            //�޸�
            0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //3 

            //���ʸ�
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            -0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3 

            //����
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, //3 


            //�Ʒ���
            0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3 
        };

        GLuint indices[] = {
            //�ո�
            0, 1, 2,
            0, 2, 3,
            //�����ʸ�
            4, 5, 6,
            4, 6, 7,
            //�޸�
            8, 10, 9,
            8, 11, 10,
            //���ʸ�
            12, 13, 14,
            12, 14, 15,
            //����
            16, 17, 18,
            16, 18, 19,
            //�Ʒ���
            20, 22, 21,
            20, 23, 22
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // ��ġ �Ӽ�(location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // ���� �Ӽ�(location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // �ؽ��� ��ǥ �Ӽ�(location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //EBO�� �����ϰ� indices�� �����Ѵ�.
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //--------------------------
        // �ؽ��� ���� ó��;

        // �ؽ�ó �ε�
        const char* textureFiles[6] = {
            "side1.jpg", "side2.jpg", "side3.jpg",
            "side4.jpg", "side5.jpg", "side6.jpg"
        };

        int width[10], height[10], nrChannels[10];
        //�ؽ��� ��ü ����� ���ε�
        glGenTextures(10, texture);

        // ���̽� �ؽ���
        unsigned char* data0 = stbi_load("container.jpg", &width[0], &height[0], &nrChannels[0], 0);

        if (data0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data0);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data0);

        //�ؽ��� ���ø�/���͸� ����
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        for (int i = 0; i < 6; ++i) {
            glBindTexture(GL_TEXTURE_2D, texture[i+1]);
            unsigned char* data = stbi_load(textureFiles[i], &width[i+1], &height[i+1], &nrChannels[i+1], 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i+1], height[i+1], 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }



    }
    // ���ø����̼� ���� �� ȣ��ȴ�.
    virtual void shutdown() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(rendering_program1);
    }

    // ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
    virtual void render(double currentTime)
    {
        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);
		glEnable(GL_CULL_FACE);


        //���������� ������ ���α׷� ��ü�� ����ϵ��� �Ѵ�.
        glUseProgram(rendering_program1);

        float angle = currentTime;
        vmath::mat4 rm = vmath::rotate(-(angle*100), 0.0f, 1.0f, 0.0f);
        vmath::mat4 tm = vmath::translate((float)sin(angle), 0.0f, (float)cos(angle));
		vmath::mat4 modelM =  rm*tm;

        vmath::vec3 eye(0.0, 1.0, -2.0);
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::mat4 perspM = vmath::lookat(eye, target, up);

        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);

		vmath::mat4 mvpM = projM * perspM * modelM;

        GLint mvpMLocation = glGetUniformLocation(rendering_program1, "mvpM");
        glUniformMatrix4fv(mvpMLocation, 1, GL_FALSE, mvpM);

        // �ؽ��ĸ� bind �Ѵ�.
        glBindVertexArray(VAO);
        for (int i = 0; i < 6; ++i) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glUniform1i(glGetUniformLocation(rendering_program1, "textures[0]"), 0);
        }

		//�̸� ������ VAO�� �׸��� ���� ���ε��Ѵ�.
        glBindVertexArray(VAO);

        //EBO�� Ȱ���� �簢���� �׸���.(�ﰢ�� �ΰ�)
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    }

private:
    GLuint rendering_program1;
    GLuint VAO, VBO, EBO;
    GLuint texture[6];
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)