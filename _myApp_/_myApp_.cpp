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
            0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,

            -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f,

            //�����ʸ�
            0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f,

            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f,

            //�޸�
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,

            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f,

            //���ʸ�
            -0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f,

            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 1.0f,

            //����
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f,
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f,

            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f,
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f

            //�Ʒ���
            - 0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f,
            0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f,
            0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f,

            0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f,
            -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f,
            -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f
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

       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindVertexArray(0);

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
		vmath::mat4 modelMatrix =  rm*tm;
        GLint modelMatLocation = glGetUniformLocation(rendering_program1, "modelMatrix");
        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, modelMatrix);

        vmath::vec3 eye(0.0, 1.0, 2.0);
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::mat4 perspM = vmath::lookat(eye, target, up);
        GLint perspMatLocation = glGetUniformLocation(rendering_program1, "LookAtMatrix");
        glUniformMatrix4fv(perspMatLocation, 1, GL_FALSE, perspM);

        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);
        GLint projMatLocation = glGetUniformLocation(rendering_program1, "projectionMatrix");
        glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, projM);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,36);

    }

private:
    GLuint rendering_program1;
    GLuint VAO, VBO, EBO;
    GLuint texture1;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)