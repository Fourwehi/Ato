/*
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// sb7.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>

// sb7::application�� ��ӹ޴´�.
class my_application : public sb7::application
{
public:
    // ���̴� �������Ѵ�.
    GLuint compile_shader1(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program1;

        //���ؽ� ���̴� �ҽ� �ڵ�
        static const GLchar* vertex_shader_source[] =
        {
           "#version 430 core                                                       \n"
           "                                                                              \n"
           "layout (location = 0) in vec4 colors0;                          \n"
           "layout (location = 1) in vec4 colors1;                          \n"
           "layout (location = 2) in vec4 colors2;                          \n"
           " layout(location = 3) in float currentTime;  \n"
           "out vec4 out_color;                                                      \n"

           "void main(void)                                                          \n"
           "{                                                                             \n"
           "   const vec4 vertices[12] = vec4[12](vec4(0.0, 0.0, 0.5, 1.0), \n"
           "                                   vec4(0.5, 0.5, 0.5, 1.0),           \n"
           "                                   vec4(0.5, 0.0, 0.5, 1.0),           \n"

           "                                   vec4(0.0, 0.0, 0.5, 1.0),           \n"
           "                                   vec4(0.0, 0.5, 0.5, 1.0),           \n"
           "                                   vec4(-0.5, 0.5, 0.5, 1.0),           \n"

           "                                   vec4(0.0, 0.0, 0.5, 1.0),           \n"
           "                                   vec4(-0.5, 0.0, 0.5, 1.0),           \n"
           "                                   vec4(-0.5, -0.5, 0.5, 1.0),           \n"

           "                                   vec4(0.0, 0.0, 0.5, 1.0),           \n"
           "                                   vec4(0.5, -0.5, 0.5, 1.0),           \n"
           "                                   vec4(0.0, -0.5, 0.5, 1.0));           \n"

           " float time = currentTime;                                           \n"

           " mat4 rotation_matrix = mat4(                                     \n"
           " vec4(cos(time), sin(time), 0.0, 0.0),                                \n"
           " vec4(-sin(time), cos(time), 0.0, 0.0),                               \n"
           " vec4(0.0, 0.0, 1.0, 0.0),                                               \n"
           " vec4(0.0, 0.0, 0.0, 1.0)                                               \n"
           " );                                                                         \n"

           " mat4 translation_matrix = mat4(                                \n"
           " vec4(1.0, 0.0, 0.0, 0.0),                                              \n"
           " vec4(0.0, 1.0, 0.0, 0.0),                                             \n"
           " vec4(0.0, 0.0, 1.0, 0.0),                                               \n"
           " vec4(sin(time) * 0.5, 0.0, 0.0, 1.0)                       \n"
           ");                                                                          \n"

           " gl_Position = translation_matrix * rotation_matrix * vertices[gl_VertexID]; \n"

           " if (gl_VertexID % 3 == 0)   out_color = colors0;              \n"
           " if (gl_VertexID % 3 == 1)   out_color = colors1;              \n"
           " if (gl_VertexID % 3 == 2)   out_color = colors2;              \n"
           " }                                                                           \n"
        };

        //�����׸�Ʈ ���̴� �ҽ� �ڵ�
        static const GLchar* fragment_shader_source[] =
        {
           "#version 430 core                             \n"
           "                                                    \n"
           "in vec4 out_color;                                 \n"
           "out vec4 color;                                 \n"
           "                                                    \n"
           "void main(void)                                \n"
           "{                                                   \n"
           "   color = out_color;          \n"
           "}                                                   \n"
        };

        //���ؽ� ���̴��� �����ϰ� �������Ѵ�.
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        glCompileShader(vertex_shader);


        //�����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

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

        //���ؽ� ���̴� �ҽ� �ڵ�
        static const GLchar* vertex_shader_source[] =
        {
           "#version 430 core                                                       \n"
           "                                                                              \n"
           " layout(location = 3) in float currentTime;  \n"
           "void main(void)                                                          \n"
           "{                                                                             \n"
           "   const vec4 vertices[6] = vec4[6](vec4(0.01, 0.0, 0.5, 1.0),    \n"
           "                                   vec4(0.01, -1.0, 0.5, 1.0),           \n"
           "                                   vec4(-0.01, -1.0, 0.5, 1.0),           \n"

           "                                   vec4(-0.01, 0.0, 0.5, 1.0),             \n"
           "                                   vec4(-0.01, -1.0, 0.5, 1.0),           \n"
           "                                   vec4(0.01, 0.0, 0.5, 1.0));           \n"


           " float time = currentTime;                                           \n"

           " mat4 rotation_matrix = mat4(                                     \n"
           " vec4(cos(time), sin(time), 0.0, 0.0),                                \n"
           " vec4(-sin(time), cos(time), 0.0, 0.0),                               \n"
           " vec4(0.0, 0.0, 1.0, 0.0),                                               \n"
           " vec4(0.0, 0.0, 0.0, 1.0)                                               \n"
           " );                                                                         \n"

           " mat4 translation_matrix = mat4(                                \n"
           " vec4(1.0, 0.0, 0.0, 0.0),                                              \n"
           " vec4(0.0, 1.0, 0.0, 0.0),                                             \n"
           " vec4(0.0, 0.0, 1.0, 0.0),                                              \n"
           " vec4(sin(time) * 0.5, 0.0, 0.0, 1.0)                                \n"
           ");                                                                          \n"

           " gl_Position = translation_matrix * vertices[gl_VertexID]; \n"
           "}                                                                           \n"
        };

        //�����׸�Ʈ ���̴� �ҽ� �ڵ�
        static const GLchar* fragment_shader_source[] =
        {
           "#version 430 core                             \n"
           "                                                    \n"
           "out vec4 color;                                 \n"
           "                                                    \n"
           "void main(void)                                \n"
           "{                                                   \n"
           "   color = vec4(1.0, 0.0, 1.0, 1.0);          \n"
           "}                                                   \n"
        };

        //���ؽ� ���̴��� �����ϰ� �������Ѵ�.
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        glCompileShader(vertex_shader);


        //�����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

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
        const GLfloat colors0[] = { (float)cos(currentTime) * 0.5f + 1.0f, 0.0f, 0.0f, 1.0f };
        const GLfloat colors1[] = { 0.0f, (float)sin(currentTime) * 0.5f + 0.6f, 0.0f, 1.0f };
        const GLfloat colors2[] = { 0.0f, 0.0f, (float)cos(currentTime) * 1.0f + 1.0f, 1.0f };
        glVertexAttrib4fv(0, colors0);
        glVertexAttrib4fv(1, colors1);
        glVertexAttrib4fv(2, colors2);


        glVertexAttrib1f(3, (float)currentTime);

        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(rendering_program2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //���������� ������ ���α׷� ��ü�� ����ϵ��� �Ѵ�.
        glUseProgram(rendering_program1);
        //�ﰢ���� �ϳ� �׸���.
        glDrawArrays(GL_TRIANGLES, 0, 12);
    }

private:
    GLuint rendering_program1;
    GLuint rendering_program2;
    GLuint vertex_array_object;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)

*/