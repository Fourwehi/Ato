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
	GLuint compile_shaders(void) {
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		//버텍스 쉐이더 소스 코드
		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core                                                       \n"
			"                                                                              \n"
			"void main(void)                                                          \n"
			"{                                                                             \n"
			"	const vec4 vertices[12] = vec4[12](vec4(0.0, 0.0, 0.5, 1.0), \n"
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
			"   gl_Position = vertices[gl_VertexID];                               \n"
			"}                                                                             \n"
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
			"	color = vec4(0.0, 0.8, 1.0, 1.0);          \n"
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
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		//이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;

	}


	//애플리케이션 초기화 수행한다.
	virtual void startup() {
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	// 애플리케이션 끝날 때 호출된다.
	virtual void shutdown() {
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
	}

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
	virtual void render(double currentTime)
	{
		const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, (float)sin(currentTime) * 0.5f + 0.5f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);

		//렌더링위해 생성한 프로그램 객체를 사용하도록 한다.
		glUseProgram(rendering_program);

		//삼각형을 하나 그린다.
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)

*/

