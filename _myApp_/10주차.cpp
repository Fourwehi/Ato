/*
// sb6.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	void load_texture(GLuint textureID, char const* filename, GLenum glColorFormat = GL_RGB, GLenum inputColorFormat = GL_RGB)
	{
		// �ؽ�ó ��ü ����� ���ε�		
		glBindTexture(GL_TEXTURE_2D, textureID);

		// �ؽ�ó �̹��� �ε��ϱ�
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, glColorFormat, width, height, 0, inputColorFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// �ؽ�ó ���ø�/���͸� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// ���ø����̼� �ʱ�ȭ �����Ѵ�.
	virtual void startup()
	{
		// ���̴� ���α׷� ������ �� ����
		shader_programs[0] = compile_shader("basic_texturing_vs.glsl", "basic_texturing_fs.glsl");
		shader_programs[1] = compile_shader("basic_lighting_vs.glsl", "basic_lighting_fs.glsl");
		shader_programs[2] = compile_shader("simple_color_vs.glsl", "simple_color_fs.glsl");

		// VAO, VBO, EBO, texture ����
		glGenVertexArrays(3, VAOs);
		glGenBuffers(3, VBOs);
		glGenBuffers(2, EBOs);
		glGenTextures(3, textures);

		stbi_set_flip_vertically_on_load(true);

		// ù ��° ��ü ���� : �ٴ� --------------------------------------------------
		glBindVertexArray(VAOs[0]);
		// �ٴ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float floor_s = 3.0f, floor_t = 3.0f;
		GLfloat floor_vertices[] = {
			1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, floor_s, floor_t,  // ���� ���
			-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, floor_t,  // ���� ���
			-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // ���� �ϴ�
			1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, floor_s, 0.0f   // ���� �ϴ�
		};

		// �ﰢ������ �׸� �ε����� �����Ѵ�.
		GLuint floor_indices[] = {
			0, 1, 2,	// ù��° �ﰢ��
			0, 2, 3		// �ι�° �ﰢ��
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// �÷� �Ӽ� (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// �ؽ�ó ��ǥ �Ӽ� (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// EBO�� �����ϰ� indices ������ ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// �ؽ�ó ��ü ����� ���ε�			
		glBindTexture(GL_TEXTURE_2D, textures[0]);

		// �ؽ�ó �̹��� �ε��ϱ�
		load_texture(textures[0], "wall.jpg", GL_RGB, GL_RGB);

		// �� ��° ��ü ���� : �ڽ� --------------------------------------------------
		glBindVertexArray(VAOs[1]);
		// �ڽ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float box_s = 1.0f, box_t = 1.0f;
		GLfloat box_vertices[] = {
			// �޸�
			-0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 0.0f,		box_s, box_t,	0.0f, 0.0f, -1.0f,
			0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
			-0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f,		box_s, 0.0f,	0.0f, 0.0f, -1.0f,

			0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
			-0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 0.0f,		box_s, box_t,	0.0f, 0.0f, -1.0f,
			0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 0.0f,		0.0f, box_t,	0.0f, 0.0f, -1.0f,
			// ������
			0.25f, 0.0f, -0.25f, 0.0f, 1.0f, 0.0f,		box_s, 0.0f,	1.0f, 0.0f, 0.0f,
			0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 0.0f,		box_s, box_t,	1.0f, 0.0f, 0.0f,
			0.25f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

			0.25f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 0.0f,		box_s, box_t,	1.0f, 0.0f, 0.0f,
			0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 0.0f,		0.0f, box_t,	1.0f, 0.0f, 0.0f,
			// ����
			0.25f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,		box_s, 0.0f,	0.0f, 0.0f, 1.0f,
			0.25f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f,		box_s, box_t,	0.0f, 0.0f, 1.0f,
			-0.25f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,

			-0.25f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.25f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f,		box_s, box_t,	0.0f, 0.0f, 1.0f,
			-0.25f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f,		0.0f, box_t,	0.0f, 0.0f, 1.0f,
			// ������
			-0.25f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f,		box_s, 0.0f,	-1.0f, 0.0f, 0.0f,
			-0.25f, 0.5f, 0.25f, 1.0f, 0.0f, 1.0f,		box_s, box_t,	-1.0f, 0.0f, 0.0f,
			-0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,

			-0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
			-0.25f, 0.5f, 0.25f, 1.0f, 0.0f, 1.0f,		box_s, box_t,	-1.0f, 0.0f, 0.0f,
			-0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 1.0f,		0.0f, box_t,	-1.0f, 0.0f, 0.0f,
			// �ٴڸ�
			-0.25f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f,		box_s, 0.0f,	0.0f, -1.0f, 0.0f,
			0.25f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f,		0.0f, box_t,	0.0f, -1.0f, 0.0f,
			0.25f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,

			0.25f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f,		0.0f, box_t,	0.0f, -1.0f, 0.0f,
			-0.25f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f,		box_s, 0.0,		0.0f, -1.0f, 0.0f,
			-0.25f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f,		box_s, box_t,	0.0f, -1.0f, 0.0f,
			// ����
			-0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 1.0f,		0.0f, box_t,	0.0f, 1.0f, 0.0f,
			0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 1.0f,		box_s, 0.0f,	0.0f, 1.0f, 0.0f,
			0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 1.0f,		box_s, box_t,	0.0f, 1.0f, 0.0f,

			0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 1.0f,		box_s, 0.0f,	0.0f, 1.0f, 0.0f,
			-0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 1.0f,		0.0f, box_t,	0.0f, 1.0f, 0.0f,
			-0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f
		};

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

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// �÷� �Ӽ� (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// �ؽ�ó ��ǥ �Ӽ� (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		// ��� �Ӽ� (location = 3)
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Diffuse Map �̹��� �ε� �� ����
		load_texture(textures[1], "container2.png", GL_RGB, GL_RGBA);

		// Specular Map �̹��� �ε� �� ����
		load_texture(textures[2], "container2_specular.png", GL_RGB, GL_RGBA);


		//  �� ��° ��ü ���� : �Ƕ�̵� --------------------------------------------------
		glBindVertexArray(VAOs[2]);
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

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// EBO�� �����ϰ� indices ������ ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	// ���ø����̼� ���� �� ȣ��ȴ�.
	virtual void shutdown()
	{
		glDeleteTextures(3, textures);
		glDeleteBuffers(2, EBOs);
		glDeleteBuffers(3, VBOs);
		glDeleteVertexArrays(3, VAOs);
		glDeleteProgram(shader_programs[0]);
		glDeleteProgram(shader_programs[1]);
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

		GLint uniform_transform1 = glGetUniformLocation(shader_programs[0], "transform");
		GLint uniform_transform2 = glGetUniformLocation(shader_programs[1], "transform");

		// ī�޶� ��Ʈ���� ���
		float distance = 2.f;
		vmath::vec3 eye((float)cos(currentTime * 0.1f) * distance, 1.0, (float)sin(currentTime * 0.1f) * distance);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / info.windowHeight, 0.1f, 1000.0f);

		// �ٴ� �׸��� ---------------------------------------
		/*
		glUseProgram(shader_programs[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[0], "transform"), 1, GL_FALSE, projM*lookAt*vmath::scale(1.5f));
		glUniform1i(glGetUniformLocation(shader_programs[0], "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */

		/*
		// ������ ���� ---------------------------------------
		vmath::vec3 lightPos = vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f);// (0.0f, 0.5f, 0.0f);
		vmath::vec3 lightColor = vmath::vec3(1.0f);
		vmath::vec3 ambientCol = lightColor * 0.2f;
		vmath::vec3 diffuseCol = lightColor * 0.5f;

		// basic_lighting ���̴� ���
		glUseProgram(shader_programs[1]);
		
		// ���ϴ� shininess �� ���� (��: 32.0)
		float shininess = 32.0f;
		glUniform1f(glGetUniformLocation(shader_programs[1], "material.shininess"), shininess);

		// diffuse�� texture unit 0, specular�� texture unit 1
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader_programs[1], "material.diffuse"), 0);
		glBindTexture(GL_TEXTURE_2D, textures[1]);  
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(shader_programs[1], "material.specular"), 1);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		
		// Light ����ü uniform
		glUniform3fv(glGetUniformLocation(shader_programs[1], "light.position"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "light.ambient"), 1, ambientCol);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "light.diffuse"), 1, diffuseCol);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "light.specular"), 1, lightColor);

		vmath::vec3 viewPos = eye;
		vmath::vec3 boxColor(1.0f, 0.5f, 0.31f);

		// �ڽ� �׸��� ---------------------------------------
		vmath::mat4 transM = vmath::translate(vmath::vec3((float)sin(currentTime * 0.5f), 0.0f, (float)cos(currentTime * 0.5f) * 0.7f));
		float angle = currentTime * 100;
		vmath::mat4 rotateM = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);

		glUseProgram(shader_programs[1]);

		glUniformMatrix4fv(glGetUniformLocation(shader_programs[1], "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[1], "view"), 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[1], "model"), 1, GL_FALSE, rotateM);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "lightPos"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "viewPos"), 1, viewPos);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "lightColor"), 1, lightColor);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "objectColor"), 1, boxColor);

		glBindVertexArray(VAOs[1]);

		for (int i = 0; i < boxPositions.size(); i++)
		{
			float angle = 20.f * i;
			vmath::mat4 model = vmath::translate(boxPositions[i]) *
				vmath::rotate(angle, 1.0f, 0.3f, 0.5f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_programs[1], "model"), 1, GL_FALSE, model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// ���� �׸��� ---------------------------------------
		float move_y = (float)cos(currentTime) * 0.2f + 0.5f;
		float scaleFactor = 0.05f;// (float)cos(currentTime)*0.05f + 0.2f;
		vmath::mat4 transform = vmath::translate(lightPos) *
			vmath::rotate(angle * 0.5f, 0.0f, 1.0f, 0.0f) *
			vmath::scale(scaleFactor, scaleFactor, scaleFactor);


		// ���� �ؽ�ó ����
		glUseProgram(shader_programs[2]);

		glUniform3fv(glGetUniformLocation(shader_programs[2], "color"), 1, lightColor);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "view"), 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "model"), 1, GL_FALSE, transform);

		glBindVertexArray(VAOs[2]);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

	}

	void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);
	}

private:
	GLuint shader_programs[3];
	GLuint VAOs[3], VBOs[3], EBOs[2];
	GLuint textures[3];

	std::vector<vmath::vec3> boxPositions;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)
*/