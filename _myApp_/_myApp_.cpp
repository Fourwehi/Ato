// sb6.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"

// sb6::application�� ��ӹ޴´�.
class my_application : public sb7::application
{
public:
	// ���̴� ���α׷� �������Ѵ�.
	GLuint compile_shader(const char* vs_file, const char* fs_file)
	{
		// ���ؽ� ���̴��� �����ϰ� �������Ѵ�.
		GLuint vertex_shader = sb7::shader::load(vs_file, GL_VERTEX_SHADER);

		// �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
		GLuint fragment_shader = sb7::shader::load(fs_file, GL_FRAGMENT_SHADER);

		// ���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// ���� ���α׷��� ���̴��� �����ϹǷ� ���̴��� �����Ѵ�.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	// ���ø����̼� �ʱ�ȭ �����Ѵ�.
	virtual void startup()
	{
		// ���̴� ���α׷� ������ �� ����
		shader_program = compile_shader("simple_phong_vs.glsl", "simple_phong_fs.glsl");

		stbi_set_flip_vertically_on_load(true);

		// �� ��° ��ü ���� : �ڽ� --------------------------------------------------
		// �ڽ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float box_s = 1.0f, box_t = 1.0f;
		GLfloat box_pos[] = {
			// �޸�
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, -0.25f,

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, -0.25f,
			// ������
			0.25f, 0.0f, -0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, 0.25f,

			0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,
			// ����
			0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, 0.25f,
			-0.25f, 0.0f, 0.25f,

			-0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			// ������
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.0f, -0.25f,

			-0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,
			// �ٴڸ�
			-0.25f, 0.0f, 0.25f,
			0.25f, 0.0f, -0.25f,
			0.25f, 0.0f, 0.25f,

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.0f, -0.25f,
			// ����
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,
			0.25f, 0.5f, -0.25f,

			0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,
			-0.25f, 0.5f, 0.25f,
		};

		GLfloat box_tex[] = {

			box_s, box_t,
			0.0f, 0.0f,
			box_s, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			box_s, box_t,
			0.0f, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			box_s, box_t,
			0.0f, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			box_s, box_t,
			0.0f, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			0.0f, box_t,
			0.0f, 0.0f,

			0.0f, box_t,
			box_s, 0.0,
			box_s, box_t,

			0.0f, box_t,
			box_s, 0.0f,
			box_s, box_t,

			box_s, 0.0f,
			0.0f, box_t,
			0.0f, 0.0f,
		};

		GLfloat box_norm[] = {

			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,

			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		box.init();
		box.setupMesh(36, box_pos, box_tex, box_norm);
		box.loadDiffuseMap("container2.png");
		box.loadSpecularMap("container2_specular.png");

		// �ڽ� 10�� ������ ����
		boxPositions.push_back(vmath::vec3(0.0f, 0.0f, 0.0f));
		boxPositions.push_back(vmath::vec3(2.0f, 5.0f, -15.0f));
		boxPositions.push_back(vmath::vec3(-1.5f, -2.2f, -2.5f));
		boxPositions.push_back(vmath::vec3(-3.8f, -2.0f, -12.3f));
		boxPositions.push_back(vmath::vec3(2.4f, -0.4f, -3.5f));
		boxPositions.push_back(vmath::vec3(-1.7f, 3.0f, -7.5f));
		boxPositions.push_back(vmath::vec3(1.3f, -2.0f, -2.5f));
		boxPositions.push_back(vmath::vec3(1.5f, 2.0f, -2.5f));
		boxPositions.push_back(vmath::vec3(1.5f, 0.2f, -1.5f));
		boxPositions.push_back(vmath::vec3(-1.3f, 1.0f, -1.5f));


		//  �� ��° ��ü ���� : �Ƕ�̵� --------------------------------------------------
		// �Ƕ�̵� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		GLfloat pyramid_vertices[] = {
			1.0f, 0.0f, -1.0f,    // ���� ���
			-1.0f, 0.0f, -1.0f,   // ���� ���
			-1.0f, 0.0f, 1.0f,    // ���� �ϴ�
			1.0f, 0.0f, 1.0f,     // ���� �ϴ�
			0.0f, 1.0f, 0.0f,      // ��� ������
			0.0f, -1.0f, 0.0f,      // �ϴ� ������
		};

		// �ﰢ������ �׸� �ε����� �����Ѵ�.
		GLuint pyramid_indices[] = {
			4, 0, 1,
			4, 1, 2,
			4, 2, 3,
			4, 3, 0,

			5, 1, 0,
			5, 2, 1,
			5, 3, 2,
			5, 0, 3,
		};

		pyramid.init();
		pyramid.setupMesh(6, pyramid_vertices);
		pyramid.setupIndices(24, pyramid_indices);
	}

	// ���ø����̼� ���� �� ȣ��ȴ�.
	virtual void shutdown()
	{
		glDeleteProgram(shader_program);
	}

	// ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
	virtual void render(double currentTime)
	{
		//currentTime = 1.46;
		//const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glViewport(0, 0, info.windowWidth, info.windowHeight);

		// ī�޶� ��Ʈ���� ���
		float distance = 2.f;
		vmath::vec3 eye((float)cos(currentTime * 0.1f) * distance, 1.0, (float)sin(currentTime * 0.1f) * distance);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / info.windowHeight, 0.1f, 1000.0f);


		// ������ ���� ---------------------------------------
		vmath::vec3 lightPos = vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f);// (0.0f, 0.5f, 0.0f);
		vmath::vec3 lightColor(1.0f, 1.0f, 1.0f);
		vmath::vec3 viewPos = eye;

		// �ڽ� �׸��� ---------------------------------------
		float angle = currentTime * 100;
		vmath::mat4 rotateM = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);

		glUseProgram(shader_program);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, lookAt);

		glUniform3fv(glGetUniformLocation(shader_program, "viewPos"), 1, viewPos);

		vmath::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
		vmath::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
		vmath::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(shader_program, "light.position"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(shader_program, "light.ambient"), 1, lightAmbient);
		glUniform3fv(glGetUniformLocation(shader_program, "light.diffuse"), 1, lightDiffuse);
		glUniform3fv(glGetUniformLocation(shader_program, "light.specular"), 1, lightSpecular);

		for (int i = 0; i < boxPositions.size(); i++)
		{
			float angle = 20.f * i;
			vmath::mat4 model = vmath::translate(boxPositions[i]) *
				vmath::rotate(angle, 1.0f, 0.3f, 0.5f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			box.draw(shader_program);
		}


		// �Ƕ�̵� (����) �׸��� ---------------------------------------
		float move_y = (float)cos(currentTime) * 0.2f + 0.5f;
		float scaleFactor = 0.05f;// (float)cos(currentTime)*0.05f + 0.2f;
		vmath::mat4 transform = vmath::translate(lightPos) *
			vmath::rotate(angle * 0.5f, 0.0f, 1.0f, 0.0f) *
			vmath::scale(scaleFactor, scaleFactor, scaleFactor);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, transform);

		pyramid.draw(shader_program);
	}

	void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);
	}

private:
	GLuint shader_program;
	std::vector<vmath::vec3> boxPositions;
	Model box, pyramid;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)