/*
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// sb7.h 헤더 파일을 포함시킨다.
#include <sb7.h>
#include <vmath.h>

// sb7::application을 상속받는다.
class my_application : public sb7::application
{
public:
    // 쉐이더 컴파일한다.
    GLuint compile_shader1(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program1;

        //버텍스 쉐이더 소스 코드
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

        //프래그먼트 쉐이더 소스 코드
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

        //버텍스 쉐이더를 생성하고 컴파일한다.
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        glCompileShader(vertex_shader);


        //프래그먼트 쉐이더를 생성하고 컴파일한다.
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        //프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
        program1 = glCreateProgram();
        glAttachShader(program1, vertex_shader);
        glAttachShader(program1, fragment_shader);
        glLinkProgram(program1);

        //이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program1;

    }

    GLuint compile_shader2(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program2;

        //버텍스 쉐이더 소스 코드
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

        //프래그먼트 쉐이더 소스 코드
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

        //버텍스 쉐이더를 생성하고 컴파일한다.
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        glCompileShader(vertex_shader);


        //프래그먼트 쉐이더를 생성하고 컴파일한다.
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        //프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
        program2 = glCreateProgram();
        glAttachShader(program2, vertex_shader);
        glAttachShader(program2, fragment_shader);
        glLinkProgram(program2);

        //이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program2;

    }


    //애플리케이션 초기화 수행한다.
    virtual void startup() {
        rendering_program1 = compile_shader1();
        rendering_program2 = compile_shader2();
        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    // 애플리케이션 끝날 때 호출된다.
    virtual void shutdown() {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program1);
        glDeleteProgram(rendering_program2);
    }

    // 렌더링 virtual 함수를 작성해서 오버라이딩한다.
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

        //렌더링위해 생성한 프로그램 객체를 사용하도록 한다.
        glUseProgram(rendering_program1);
        //삼각형을 하나 그린다.
        glDrawArrays(GL_TRIANGLES, 0, 12);
    }

private:
    GLuint rendering_program1;
    GLuint rendering_program2;
    GLuint vertex_array_object;
};

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)

*/