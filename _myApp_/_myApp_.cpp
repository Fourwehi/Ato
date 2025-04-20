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
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
           -0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
           -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3  


            //오른쪽면
            0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3 

            //뒷면
            0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //3 

            //왼쪽면
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            -0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3 

            //윗면
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, //3 


            //아랫면
            0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3 
        };

        GLuint indices[] = {
            //앞면
            0, 1, 2,
            0, 2, 3,
            //오른쪽면
            4, 5, 6,
            4, 6, 7,
            //뒷면
            8, 10, 9,
            8, 11, 10,
            //왼쪽면
            12, 13, 14,
            12, 14, 15,
            //윗면
            16, 17, 18,
            16, 18, 19,
            //아랫면
            20, 22, 21,
            20, 23, 22
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
        // 텍스쳐 좌표 속성(location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //EBO를 생성하고 indices를 복사한다.
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //--------------------------
        // 텍스쳐 관련 처리;

        // 텍스처 로딩
        const char* textureFiles[6] = {
            "side1.jpg", "side2.jpg", "side3.jpg",
            "side4.jpg", "side5.jpg", "side6.jpg"
        };

        int width[10], height[10], nrChannels[10];
        //텍스쳐 객체 만들고 바인딩
        glGenTextures(10, texture);

        // 베이스 텍스쳐
        unsigned char* data0 = stbi_load("container.jpg", &width[0], &height[0], &nrChannels[0], 0);

        if (data0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data0);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data0);

        //텍스쳐 샘플링/필터링 설정
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
		vmath::mat4 modelM =  rm*tm;

        vmath::vec3 eye(0.0, 1.0, -2.0);
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::mat4 perspM = vmath::lookat(eye, target, up);

        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);

		vmath::mat4 mvpM = projM * perspM * modelM;

        GLint mvpMLocation = glGetUniformLocation(rendering_program1, "mvpM");
        glUniformMatrix4fv(mvpMLocation, 1, GL_FALSE, mvpM);

        // 텍스쳐를 bind 한다.
        glBindVertexArray(VAO);
        for (int i = 0; i < 6; ++i) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glUniform1i(glGetUniformLocation(rendering_program1, "textures[0]"), 0);
        }

		//미리 설정한 VAO를 그리기 위해 바인딩한다.
        glBindVertexArray(VAO);

        //EBO를 활용해 사각형을 그린다.(삼각형 두개)
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    }

private:
    GLuint rendering_program1;
    GLuint VAO, VBO, EBO;
    GLuint texture[6];
};

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)