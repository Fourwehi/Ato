// sb6.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// sb6::application�� ��ӹ޴´�.
class my_application : public sb7::application
{
public:
	// ���̴� �������Ѵ�.
	GLuint compile_shader1(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		// ���ؽ� ���̴� �ҽ� �ڵ�
		static const GLchar * vertex_shader_source[] =
		{
			"#version 430 core											\n"
			"															\n"
			"layout(location = 0) in vec3 pos;							\n"
			"layout(location = 1) in vec3 color;						\n"
			"layout(location = 2) in vec2 texCoord;						\n"
			"															\n"
			"uniform mat4 transform;									\n"
			"															\n"
			"out vec3 vsColor;											\n"
			"out vec2 vsTexCoord;										\n"
			"															\n"
			"void main(void)											\n"
			"{															\n"
			"	gl_Position = transform*vec4(pos.x, pos.y, pos.z, 1.0);	\n"
			"															\n"
			"	vsColor = color;										\n"
			"	vsTexCoord = texCoord;									\n"
			"}															\n"
		};

		// �����׸�Ʈ ���̴� �ҽ� �ڵ�
		static const GLchar * fragment_shader_source[] =
		{
			"#version 430 core								\n"
			"												\n"
			"in vec3 vsColor;								\n"
			"in vec2 vsTexCoord;							\n"
			"uniform sampler2D texture1;					\n"
			"												\n"
			"out vec4 fragColor;							\n"
			"												\n"
			"void main(void)								\n"
			"{												\n"
			"	fragColor = texture(texture1, vsTexCoord);	\n"
			"}												\n"
		};

		// ���ؽ� ���̴��� �����ϰ� �������Ѵ�.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// ���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// ���� ���α׷��� ���̴��� �����ϹǷ� ���̴��� �����Ѵ�.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shader2(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		// ���ؽ� ���̴� �ҽ� �ڵ�
		static const GLchar * vertex_shader_source[] =
		{
			"#version 430 core											\n"
			"															\n"
			"layout(location = 0) in vec3 pos;							\n"
			"layout(location = 1) in vec3 color;						\n"
			"															\n"
			"uniform mat4 transform;									\n"
			"															\n"
			"out vec3 vsColor;											\n"
			"															\n"
			"void main(void)											\n"
			"{															\n"
			"	gl_Position = transform*vec4(pos.x, pos.y, pos.z, 1.0);	\n"
			"															\n"
			"	vsColor = color;										\n"
			"}															\n"
		};

		// �����׸�Ʈ ���̴� �ҽ� �ڵ�
		static const GLchar * fragment_shader_source[] =
		{
			"#version 430 core						\n"
			"in vec3 vsColor;						\n"
			"out vec4 fragColor;					\n"
			"										\n"
			"void main(void)						\n"
			"{										\n"
			"	fragColor = vec4(vsColor, 1.0);		\n"
			"}										\n"
		};

		// ���ؽ� ���̴��� �����ϰ� �������Ѵ�.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// ���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
		program = glCreateProgram();
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
		shader_programs[0] = compile_shader1();
		shader_programs[1] = compile_shader2();

		// VAO, VBO, EBO, texture ����
		glGenVertexArrays(3, VAOs);
		glGenBuffers(3, VBOs);
		glGenBuffers(2, EBOs);
		glGenTextures(2, textures);

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
		int width, height, nrChannels;
		unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// �ؽ�ó ���ø�/���͸� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// �� ��° ��ü ���� : �ڽ� --------------------------------------------------
		glBindVertexArray(VAOs[1]);
		// �ڽ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float box_s = 1.0f, box_t = 1.0f;
		GLfloat box_vertices[] = {
			// �޸�
			-0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 0.0f, box_s, box_t,
			0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			-0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f, box_s, 0.0f,

			0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			-0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 0.0f, box_s, box_t,
			0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 0.0f, 0.0f, box_t,
			// ������
			0.25f, 0.0f, -0.25f, 0.0f, 1.0f, 0.0f, box_s, 0.0f,
			0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 0.0f, box_s, box_t,
			0.25f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

			0.25f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 0.0f, box_s, box_t,
			0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, box_t,
			// ����
			0.25f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f, box_s, 0.0f,
			0.25f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f, box_s, box_t,
			-0.25f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.25f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.25f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f, box_s, box_t,
			-0.25f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, box_t,
			// ������
			-0.25f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f, box_s, 0.0f,
			-0.25f, 0.5f, 0.25f, 1.0f, 0.0f, 1.0f, box_s, box_t,
			-0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.25f, 0.0f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.25f, 0.5f, 0.25f, 1.0f, 0.0f, 1.0f, box_s, box_t,
			-0.25f, 0.5f, -0.25f, 1.0f, 0.0f, 1.0f, 0.0f, box_t,
			// �ٴڸ�
			-0.25f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, box_s, 0.0f,
			0.25f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, box_t,
			0.25f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

			0.25f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, box_t,
			-0.25f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, box_s, 0.0,
			-0.25f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f, box_s, box_t,
			// ����
			-0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, box_t,
			0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 1.0f, box_s, 0.0f,
			0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 1.0f, box_s, box_t,

			0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 1.0f, box_s, 0.0f,
			-0.25f, 0.5f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, box_t,
			-0.25f, 0.5f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

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

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// �ؽ�ó ��ü ����� ���ε�			
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		// �ؽ�ó �̹��� �ε��ϱ�
		width, height, nrChannels;
		data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// �ؽ�ó ���ø�/���͸� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//  �� ��° ��ü ���� : �Ƕ�̵� --------------------------------------------------
		glBindVertexArray(VAOs[2]);
		// �Ƕ�̵� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		GLfloat pyramid_vertices[] = {
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,   // ���� ���
			-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // ���� ���
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // ���� �ϴ�
			1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,    // ���� �ϴ�
			0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f      // ������
		};

		// �ﰢ������ �׸� �ε����� �����Ѵ�.
		GLuint pyramid_indices[] = {
			0, 2, 1,
			0, 3, 2,
			4, 0, 1,
			4, 1, 2,
			4, 2, 3,
			4, 3, 0
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// �÷� �Ӽ� (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// EBO�� �����ϰ� indices ������ ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// framebuffer ���� --------------------------------------------------
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// color buffer �ؽ�ó ���� �� ����
		glGenTextures(1, &FBO_texture);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.windowWidth, info.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_texture, 0);
		// depth&stencil buffer�� ���� Render Buffer Object ���� �� ����
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, info.windowWidth, info.windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); 
		// Framebuffer�� ���������� ����� ������ Ȯ��
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			glfwTerminate();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	// ���ø����̼� ���� �� ȣ��ȴ�.
	virtual void shutdown()
	{
		glDeleteTextures(2, textures);
		glDeleteBuffers(2, EBOs);
		glDeleteBuffers(3, VBOs);
		glDeleteVertexArrays(3, VAOs);
		glDeleteProgram(shader_programs[0]);
		glDeleteProgram(shader_programs[1]);
	}

	// ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
	virtual void render(double currentTime)
	{
		glEnable(GL_CULL_FACE);
		glViewport(0, 0, info.windowWidth, info.windowHeight);

		GLint uniform_transform1 = glGetUniformLocation(shader_programs[0], "transform");
		GLint uniform_transform2 = glGetUniformLocation(shader_programs[1], "transform");

		// ī�޶� ��Ʈ���� ���
		float distance = 2.f;
		vmath::vec3 eye((float)cos(currentTime*0.1f)*distance, 1.0, (float)sin(currentTime*0.1f)*distance);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;// (float)cos(currentTime)*20.f + 50.0f;
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

		// Render-To-Texture Framebuffer ���ε� ----------------------------------------
		// FBO ���ε�
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST); 
		// FBO�� ����� ���۵��� ���� �����, ���� �׽��� Ȱ��ȭ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �ڽ� �׸��� ---------------------------------------
		vmath::mat4 transM = vmath::translate(vmath::vec3((float)sin(currentTime), 0.0f, (float)cos(currentTime) * 0.7f));
		float angle = currentTime * 150;
		vmath::mat4 rotateM = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);

		glUseProgram(shader_programs[0]);
		glUniformMatrix4fv(uniform_transform1, 1, GL_FALSE, projM*lookAt*transM*rotateM);
		glUniform1i(glGetUniformLocation(shader_programs[0], "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// �ٴ� �׸��� ---------------------------------------
		glUseProgram(shader_programs[0]);
		glUniformMatrix4fv(uniform_transform1, 1, GL_FALSE, projM*lookAt*vmath::scale(1.5f));
		glUniform1i(glGetUniformLocation(shader_programs[0], "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// �Ƕ�̵� �׸��� ---------------------------------------
		float move_y = (float)cos(currentTime)*0.2f + 0.5f;
		float scaleFactor = (float)cos(currentTime)*0.05f + 0.2f;
		vmath::mat4 transform = vmath::translate(0.0f, move_y, 0.0f)*
			vmath::rotate(angle*0.5f, 0.0f, 1.0f, 0.0f)*
			vmath::scale(scaleFactor, scaleFactor, scaleFactor);

		glUseProgram(shader_programs[1]);
		glUniformMatrix4fv(uniform_transform2, 1, GL_FALSE, projM*lookAt*transform);
		glBindVertexArray(VAOs[2]);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		// ������ Framebuffer ���ε� ----------------------------------------
		// �⺻ Framebuffer�� �ǵ�����
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); 
		// ���۵��� �� �����
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		// FBO Texture�� ���̴� ���α׷��� ����
		glUseProgram(shader_programs[0]);
		glUniformMatrix4fv(uniform_transform1, 1, GL_FALSE, projM*lookAt*rotateM*vmath::scale(1.3f)*vmath::translate(0.0f, -0.25f, 0.0f));
		glUniform1i(glGetUniformLocation(shader_programs[0], "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void drawScene(double currentTime)
	{

	}

	void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);
	}

private:
	GLuint shader_programs[2];
	GLuint VAOs[3], VBOs[3], EBOs[2];
	GLuint textures[2];
	GLuint FBO, FBO_texture;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)