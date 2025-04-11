#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// sb7.h 헤더 파일을 포함시킨다.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>

// sb7::application을 상속받는다.
class my_application : public sb7::application
{
public:
    // 쉐이더 컴파일한다.
    GLuint compile_shader1(void) {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program1;

        // 버텍스 쉐이더를 생성하고 컴파일한다.
        vertex_shader = sb7::shader::load("wheel_vs.glsl", GL_VERTEX_SHADER);

        // 프래그먼트 쉐이더를 생성하고 컴파일한다.
        fragment_shader = sb7::shader::load("wheel_fs.glsl", GL_FRAGMENT_SHADER);

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

        // 버텍스 쉐이더를 생성하고 컴파일한다.
        vertex_shader = sb7::shader::load("stick_vs.glsl", GL_VERTEX_SHADER);

        // 프래그먼트 쉐이더를 생성하고 컴파일한다.
        fragment_shader = sb7::shader::load("stick_fs.glsl", GL_FRAGMENT_SHADER);

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
        const GLfloat colors0[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 1.0f, 0.0f, 1.0f };
        const GLfloat colors1[] = { (float)cos(currentTime) * 0.5f + 0.5f, (float)sin(currentTime) * 0.5f + 0.6f, 0.0f, 1.0f };
        const GLfloat colors2[] = { (float)cos(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 1.0f ,(float)cos(currentTime) * 1.0f + 1.0f, 1.0f };

        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glVertexAttrib1f(3, (float)currentTime);

        glUseProgram(rendering_program2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //렌더링위해 생성한 프로그램 객체를 사용하도록 한다.
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

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)