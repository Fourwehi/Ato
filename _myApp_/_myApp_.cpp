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
	// 광원을 위한 쉐이더 컴파일한다.
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

    //애플리케이션 초기화 수행한다.
    virtual void startup() {
        stbi_set_flip_vertically_on_load(true);

        rendering_program1 = compile_shader1();
        light_program = compile_shader_light();


        GLfloat vertices[] = {

            //앞면
            0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
           -0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
           -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3


            //오른쪽면
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3

            //뒷면
            0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //2
            0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //3

            //왼쪽면
            -0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2
            -0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, //3

            //아랫면
            0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //0
            -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
            -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //2
            0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //3

            //윗면
            0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //0
            -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //1
            -0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //2
            0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f //3
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
            //아랫면
            16, 18, 17,
            16, 19, 18,
            //윗면
            20, 21, 22,
            20, 22, 23
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 위치 속성(location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        // 색상 속성(location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        // 텍스쳐 좌표 속성(location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        //EBO를 생성하고 indices를 복사한다.
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //---------------------------
        // 텍스쳐 처리 과정

        // 텍스처 0번: container.jpg (기본 텍스처)
        baseTexture = loadTexture("container2.png");            // 디퓨즈 맵
        specularTexture = loadTexture("container2_specular.png");   // 스페큘러 맵

    }

    // 종료 시 자원 해제
    virtual void shutdown() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(rendering_program1);
    }

    // 매 프레임마다 호출되는 렌더링 함수
    virtual void render(double currentTime) {
        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black); // 화면 초기화
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 색상 및 깊이 버퍼 초기화
        glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
        glEnable(GL_CULL_FACE); // 컬링 활성화

        glUseProgram(rendering_program1); // 셰이더 프로그램 사용

        // MVP 행렬 계산
        float angle = -currentTime;
        vmath::mat4 rm = vmath::rotate((angle * -100), 0.0f, 1.0f, 0.0f);
        vmath::mat4 tm = vmath::translate((float)sin(angle), 0.0f, (float)cos(angle));
        vmath::mat4 modelM = rm;
        vmath::vec3 target(0.0, 0.0, 0.0);
        vmath::vec3 up(0.0, 1.0, 0.0);
        vmath::vec3 eye(0.0, 1.0, -2.0);
        vmath::mat4 viewM = vmath::lookat(eye, target, up);
        vmath::mat4 projM = vmath::perspective(50.0f, (float)(info.windowWidth / info.windowHeight), 0.1f, 1000.0f);

		// 모델, 뷰, 프로젝션 행렬을 각각의 uniform 변수에 바인딩
        GLint locModel = glGetUniformLocation(rendering_program1, "model");
        glUniformMatrix4fv(locModel, 1, GL_FALSE, modelM);
        GLint locView = glGetUniformLocation(rendering_program1, "view");
        glUniformMatrix4fv(locView, 1, GL_FALSE, viewM);
        GLint locProj = glGetUniformLocation(rendering_program1, "projection");
        glUniformMatrix4fv(locProj, 1, GL_FALSE, projM); 

        // 3) 매 프레임 바인딩
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, baseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);

		// 조명 속성 설정
        vmath::vec3 lightPos = vmath::vec3(-2.0f, 0.0f, 0.0f);
        vmath::vec3 lightAmbient = vmath::vec3(0.2f);
        vmath::vec3 lightDiffuse = vmath::vec3(0.5f);
        vmath::vec3 lightSpecular = vmath::vec3(1.0f);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.position"), 1, lightPos);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.ambient"), 1, lightAmbient);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.diffuse"), 1, lightDiffuse);
        glUniform3fv(glGetUniformLocation(rendering_program1, "light.specular"), 1, lightSpecular);

        // Material 세팅
        float shininess = 32.0f;
        glUniform1f(glGetUniformLocation(rendering_program1, "material.shininess"), shininess); 
        glUniform1i(glGetUniformLocation(rendering_program1, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(rendering_program1, "material.specular"), 1);

        // VAO 바인딩 후 면마다 그리기
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, baseTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);

        // 인덱스 버퍼의 위치에 따라 각 면 그리기
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

        // VAO 바인딩 해제
        glBindVertexArray(0);

        // 조명 프로그램 사용
        glUseProgram(light_program);
        vmath::mat4 model_light = vmath::scale(0.3f) *vmath::translate(lightPos)* rm ;
        glUniformMatrix4fv(glGetUniformLocation(light_program, "model"), 1, GL_FALSE, model_light);
        glUniformMatrix4fv(glGetUniformLocation(light_program, "view"), 1, GL_FALSE, viewM);
        glUniformMatrix4fv(glGetUniformLocation(light_program, "projection"), 1, GL_FALSE, projM);
        glUniform3fv(glGetUniformLocation(light_program, "lightColor"), 1, lightSpecular);

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // VAO 바인딩 해제
        glBindVertexArray(0);

    }
};

DECLARE_MAIN(my_application)
