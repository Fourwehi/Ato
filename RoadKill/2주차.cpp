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
	GLuint compile_shaders(void) {
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		//���ؽ� ���̴� �ҽ� �ڵ�
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

		//�����׸�Ʈ ���̴� �ҽ� �ڵ�
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

		//���ؽ� ���̴��� �����ϰ� �������Ѵ�.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		//�����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		//���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		//���� ���α׷��� ���̴��� �����ϹǷ� ���̴��� �����Ѵ�.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;

	}


	//���ø����̼� �ʱ�ȭ �����Ѵ�.
	virtual void startup() {
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	// ���ø����̼� ���� �� ȣ��ȴ�.
	virtual void shutdown() {
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
	}

	// ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
	virtual void render(double currentTime)
	{
		const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, (float)sin(currentTime) * 0.5f + 0.5f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, red);

		//���������� ������ ���α׷� ��ü�� ����ϵ��� �Ѵ�.
		glUseProgram(rendering_program);

		//�ﰢ���� �ϳ� �׸���.
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)

*/

