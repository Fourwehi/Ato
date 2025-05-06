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
private:
    GLuint rendering_program1;
    GLuint light_program;
    GLuint VAO, VBO, EBO;
    GLuint baseTexture;
    GLuint specularTexture;
    GLuint overlayTextures[6];


public:
    static GLuint loadTexture(const char* filepath)
    {
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return tex;
    }
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
	// ������ ���� ���̴� �������Ѵ�.
    GLuint compile_shader_light()
    {
        GLuint vs = sb7::shader::load("light_vs.glsl", GL_VERTEX_SHADER);
        GLuint fs = sb7::shader::load("light_fs.glsl", GL_FRAGMENT_SHADER);
        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return prog;
    }

    //���ø����̼� �ʱ�ȭ �����Ѵ�.
    virtual void startup() {
        stbi_set_flip_vertically_on_load(true);

        rendering_program1 = compile_shader1();
        light_program = compile_shader_light();


        GLfloat vertices[] = {

            //�ո�
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
           -0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
           -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3


            //�����ʸ�
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3

            //�޸�
            0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //2
            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //3

            //���ʸ�
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            -0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3

            //�Ʒ���
            0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3

            //����
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f //3
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
            //�Ʒ���
            16, 18, 17,
            16, 19, 18,
            //����
            20, 21, 22,
            20, 22, 23
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // ��ġ �Ӽ�(location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        // ���� �Ӽ�(location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        // �ؽ��� ��ǥ �Ӽ�(location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        //EBO�� �����ϰ� indices�� �����Ѵ�.
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //---------------------------
        // �ؽ��� ó�� ����

        // �ؽ�ó 0��: container.jpg (�⺻ �ؽ�ó)
        baseTexture = loadTexture("container2.png");            // ��ǻ�� ��
        specularTexture = loadTexture("container2_specular.png");   // ����ŧ�� ��

    }

    // ���� �� �ڿ� ����
    virtual void shutdown() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(rendering_program1);
    }

    // �� �����Ӹ��� ȣ��Ǵ� ������ �Լ�
    virtual void render(double currentTime) {
        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black); // ȭ�� �ʱ�ȭ
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ���� �� ���� ���� �ʱ�ȭ
        glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ
        glEnable(GL_CULL_FACE); // �ø� Ȱ��ȭ

        glUseProgram(rendering_program1); // ���̴� ���α׷� ���

        // MVP ��� ���
        float angle = -currentTime;
        vmath::mat4 rm = vmath::rotate((angle * -100), 0.0f, 1.0f, 0.0f);
        vmath::mat4 tm = vmath::translate((float)sin(angle), 0.0f, (float)cos(angle));
        vmath::mat4 modelM = rm;
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::vec3 eye(0.0, 1.0, -2.0);
        vmath::mat4 viewM = vmath::lookat(eye, target, up);
        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);

		// ��, ��, �������� ����� ������ uniform ������ ���ε�
        GLint locModel = glGetUniformLocation(rendering_program1, "model");
        glUniformMatrix4fv(locModel, 1, GL_FALSE, modelM);
        GLint locView = glGetUniformLocation(rendering_program1, "view");
        glUniformMatrix4fv(locView, 1, GL_FALSE, viewM);
        GLint locProj = glGetUniformLocation(rendering_program1, "projection");
        glUniformMatrix4fv(locProj, 1, GL_FALSE, projM); 

        // 3) �� ������ ���ε�
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, baseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);

		// ���� �Ӽ� ����
        vmath::vec3 lightPos = vmath::vec3(-2.0f, 0.0f, 0.0f);
        vmath::vec3 lightAmbient = vmath::vec3(0.2f);
        vmath::vec3 lightDiffuse = vmath::vec3(0.5f);
        vmath::vec3 lightSpecular = vmath::vec3(1.0f);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.position"), 1, lightPos);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.ambient"), 1, lightAmbient);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.diffuse"), 1, lightDiffuse);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.specular"), 1, lightSpecular);

        // Material ����
        float shininess = 32.0f;
        glUniform1f(glGetUniformLocation(rendering_program1, "material.shininess"), shininess); 
        glUniform1i(glGetUniformLocation(rendering_program1, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(rendering_program1, "material.specular"), 1);

        // VAO ���ε� �� �鸶�� �׸���
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, baseTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);

        // �ε��� ������ ��ġ�� ���� �� �� �׸���
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

        // VAO ���ε� ����
        glBindVertexArray(0);

        // ���� ���α׷� ���
        glUseProgram(light_program);
        vmath::mat4 model_light = vmath::scale(0.3f) *vmath::translate(lightPos)* rm ;
        glUniformMatrix4fv(glGetUniformLocation(light_program, "model"), 1, GL_FALSE, model_light);
        glUniformMatrix4fv(glGetUniformLocation(light_program, "view"), 1, GL_FALSE, viewM);
        glUniformMatrix4fv(glGetUniformLocation(light_program, "projection"), 1, GL_FALSE, projM);
        glUniform3fv(glGetUniformLocation(light_program, "lightColor"), 1, lightSpecular);

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // VAO ���ε� ����
        glBindVertexArray(0);

    }
};

DECLARE_MAIN(my_application)
