#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// sb7.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>

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
        vertex_shader = sb7::shader::load("wheel_vs.glsl", GL_VERTEX_SHADER);

        // �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
        fragment_shader = sb7::shader::load("wheel_fs.glsl", GL_FRAGMENT_SHADER);

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

    GLuint compile_shader2(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program2;

        // ���ؽ� ���̴��� �����ϰ� �������Ѵ�.
        vertex_shader = sb7::shader::load("stick_vs.glsl", GL_VERTEX_SHADER);

        // �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
        fragment_shader = sb7::shader::load("stick_fs.glsl", GL_FRAGMENT_SHADER);

        //���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
        program2 = glCreateProgram();
        glAttachShader(program2, vertex_shader);
        glAttachShader(program2, fragment_shader);
        glLinkProgram(program2);

        //���� ���α׷��� ���̴��� �����ϹǷ� ���̴��� �����Ѵ�.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program2;

    }


    //���ø����̼� �ʱ�ȭ �����Ѵ�.
    virtual void startup() {
        rendering_program1 = compile_shader1();
        rendering_program2 = compile_shader2();
        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    // ���ø����̼� ���� �� ȣ��ȴ�.
    virtual void shutdown() {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program1);
        glDeleteProgram(rendering_program2);
    }

    // ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
    virtual void render(double currentTime)
    {
        const GLfloat colors0[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 1.0f, 0.0f, 1.0f };
        const GLfloat colors1[] = { (float)cos(currentTime) * 0.5f + 0.5f, (float)sin(currentTime) * 0.5f + 0.6f, 0.0f, 1.0f };
        const GLfloat colors2[] = { (float)cos(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 1.0f ,(float)cos(currentTime) * 1.0f + 1.0f, 1.0f };

        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glVertexAttrib1f(3, (float)currentTime);

        glUseProgram(rendering_program2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //���������� ������ ���α׷� ��ü�� ����ϵ��� �Ѵ�.
        glUseProgram(rendering_program1);
        glVertexAttrib4fv(0, colors0);
        glVertexAttrib4fv(1, colors1);
        glVertexAttrib4fv(2, colors2);

        glVertexAttrib1f(3, (float)currentTime);

        glDrawArrays(GL_TRIANGLES, 0, 12);

    }

private:
    GLuint rendering_program1;
    GLuint rendering_program2;
    GLuint vertex_array_object;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)