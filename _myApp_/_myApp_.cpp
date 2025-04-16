#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#define STB_IMAGE_IMPLEMENTATION

// sb7.h 헤더 파일을 포함시킨다.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"

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
        vertex_shader = sb7::shader::load("box_vs.glsl", GL_VERTEX_SHADER);

        // 프래그먼트 쉐이더를 생성하고 컴파일한다.
        fragment_shader = sb7::shader::load("box_fs.glsl", GL_FRAGMENT_SHADER);

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

    //애플리케이션 초기화 수행한다.
    virtual void startup() {
		stbi_set_flip_vertically_on_load(true);

        rendering_program1 = compile_shader1();

        GLfloat vertices[] = {
            
            //앞면
            0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,

            -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f,

            //오른쪽면
            0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f,

            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f,

            //뒷면
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,

            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
            0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f,

            //왼쪽면
            -0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f,

            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f,
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 1.0f,

            //윗면
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f,
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f,
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f,

            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f,
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f,
            -0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f

            //아랫면
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

        // 위치 속성(location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 색상 속성(location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindVertexArray(0);

    }

    // 애플리케이션 끝날 때 호출된다.
    virtual void shutdown() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(rendering_program1);
    }

    // 렌더링 virtual 함수를 작성해서 오버라이딩한다.
    virtual void render(double currentTime)
    {
        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);
		glEnable(GL_CULL_FACE);


        //렌더링위해 생성한 프로그램 객체를 사용하도록 한다.
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

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)