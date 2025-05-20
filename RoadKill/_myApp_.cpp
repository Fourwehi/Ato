#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// sb7.h ��� ������ ���Խ�Ų��.
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
		// Ŭ���� ���� ���� �ʱ�ȭ
		initValues();

		// ���̴� ���α׷� ������ �� ����
		shader_program = compile_shader("simple_phong_vs.glsl", "simple_phong_fs.glsl");

		stbi_set_flip_vertically_on_load(true);

		// ù ��° ��ü ���� : OBJ ��  --------------------------------------------------
		objModel.init();
		objModel.loadOBJ("squid_girl.obj");
		objModel.loadDiffuseMap("squid_girl.png");

		// �� ��° ��ü ���� : �Ƕ�̵� --------------------------------------------------
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

		pyramidModel.init();
		pyramidModel.setupMesh(6, pyramid_vertices);
		pyramidModel.setupIndices(24, pyramid_indices);

		glEnable(GL_MULTISAMPLE);
	}

	// ���ø����̼� ���� �� ȣ��ȴ�.
	virtual void shutdown()
	{
		glDeleteProgram(shader_program);
	}

	// ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
	virtual void render(double currentTime)
	{
		if (!pause)
			animationTime += currentTime - previousTime;
		previousTime = currentTime;

		//const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		// ī�޶� ��Ʈ���� ���
		float distance = 5.f;
		//vmath::vec3 eye((float)cos(animationTime*0.3f)*distance, 1.0, (float)sin(animationTime*0.3f)*distance);
		vmath::vec3 eye(0.0f, 1.0f, distance);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

		// ������ ���� ---------------------------------------
		float r = 2.f;
		vmath::vec3 lightPos = vmath::vec3((float)sin(animationTime * 0.5f) * r, 0.f, (float)cos(animationTime * 0.5f) * r);
		vmath::vec3 viewPos = eye;

		vmath::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
		vmath::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
		vmath::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

		// �� �׸��� ---------------------------------------
		glUseProgram(shader_program);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, lookAt);

		glUniform3fv(glGetUniformLocation(shader_program, "viewPos"), 1, viewPos);

		glUniform3fv(glGetUniformLocation(shader_program, "light.position"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(shader_program, "light.ambient"), 1, lightAmbient);
		glUniform3fv(glGetUniformLocation(shader_program, "light.diffuse"), 1, lightDiffuse);
		glUniform3fv(glGetUniformLocation(shader_program, "light.specular"), 1, lightSpecular);


		if (lineMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (drawModel) {
			vmath::mat4 model = vmath::translate(objPosition) *
				vmath::rotate(objYangle, 0.0f, 1.0f, 0.0f) *
				vmath::scale(0.01f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			objModel.draw(shader_program);
		}

		// �Ƕ�̵� �׸��� ---------------------------------------
		if (drawLight) {
			vmath::mat4 transform = vmath::translate(lightPos) * vmath::scale(0.05f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, transform);
			pyramidModel.draw(shader_program);
		}


	}

	virtual void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);

		if (glViewport != NULL)
			glViewport(0, 0, info.windowWidth, info.windowHeight);
	}

	virtual void init()
	{
		sb7::application::init();

		info.samples = 4;
		info.flags.debug = 1;
	}

	virtual void onKey(int key, int action)
	{
		if (action == GLFW_PRESS) {
			switch (key) {
			case ' ': // �����̽���
				pause = !pause;
				break;
			case '1':
				drawModel = !drawModel;
				break;
			case '2':
				drawLight = !drawLight;
				break;
			case 'M':
				lineMode = !lineMode;
				break;
			case 'R':
				initValues();
				break;
			default:
				break;
			};
		}
	}

	virtual void onMouseButton(int button, int action)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseDown = true;

			int x, y;
			getMousePosition(x, y);
			mousePostion = vmath::vec2(float(x), float(y));
		}
		else
		{
			mouseDown = false;
		}
	}

	virtual void onMouseMove(int x, int y)
	{
		if (mouseDown)
		{
			objYangle += float(x - mousePostion[0]) / 2.f;
			mousePostion = vmath::vec2(float(x), float(y));
		}
	}

#define MAX_FOV 120.f
#define MIN_FOV 10.f
	virtual void onMouseWheel(int pos)
	{
		if (pos > 0)
			fov = vmath::min(MAX_FOV, fov + 1.0f);
		else
			fov = vmath::max(MIN_FOV, fov - 1.0f);
	}

	void initValues()
	{
		drawModel = true;
		drawLight = true;
		pause = false;
		animationTime = 0;
		previousTime = 0;
		lineMode = false;

		mouseDown = false;

		fov = 50.f;

		objPosition = vmath::vec3(0.0f, -1.0f, 0.0f);
		objYangle = 0.f;
	}


private:
	GLuint shader_program;

	Model objModel, pyramidModel;
	vmath::vec3 objPosition;
	float objYangle;

	bool drawModel, drawLight;
	bool lineMode;
	bool pause;
	double previousTime;
	double animationTime;

	vmath::vec2 mousePostion;
	bool mouseDown;

	float fov;

};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)