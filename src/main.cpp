#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

size_t TypeSize(GLenum type)
{
	size_t size;

#define CASE(Enum, Count, Type) \
	case Enum: size = Count * sizeof(Type); break

	switch (type) {
		CASE(GL_FLOAT, 1, GLfloat);
		CASE(GL_FLOAT_VEC2, 2, GLfloat);
		CASE(GL_FLOAT_VEC3, 3, GLfloat);
		CASE(GL_FLOAT_VEC4, 4, GLfloat);
		CASE(GL_INT, 1, GLint);
		CASE(GL_INT_VEC2, 2, GLint);
		CASE(GL_INT_VEC3, 3, GLint);
		CASE(GL_INT_VEC4, 4, GLint);
		CASE(GL_UNSIGNED_INT, 1, GLuint);
		CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
		CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
		CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
		CASE(GL_BOOL, 1, GLboolean);
		CASE(GL_BOOL_VEC2, 2, GLboolean);
		CASE(GL_BOOL_VEC3, 3, GLboolean);
		CASE(GL_BOOL_VEC4, 4, GLboolean);
		CASE(GL_FLOAT_MAT2, 4, GLfloat);
		CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		CASE(GL_FLOAT_MAT3, 9, GLfloat);
		CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		CASE(GL_FLOAT_MAT4, 16, GLfloat);
		CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		CASE(GL_FLOAT_MAT4x3, 12, GLfloat);

#undef CASE

	default:
		fprintf(stderr, "Unknown type: 0x%x\n", type);
		exit(EXIT_FAILURE);
		break;
	}
	return size;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "OpenGL Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW Initialization Failed!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	float positions[] = {
		-0.5F, -0.5F, //0
		 0.5F, -0.5F, //1
		 0.5F,  0.5F, //2
		-0.5F,  0.5F, //3
	};

	unsigned int indicies[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indicies, GL_STATIC_DRAW);

	Shader ordinaryShader("../res/vertex.glsl", "../res/fragment.glsl");
	ordinaryShader.use();

	/* Initialize uniform values in uniform block "Uniforms" */
	GLuint uboIndex;
	GLint uboSize;
	GLuint ubo;
	GLvoid *buffer;

	// Find the uniform buffer index for "Uniforms", and
	// determine the block’s sizes
	uboIndex = glGetUniformBlockIndex(ordinaryShader.m_ShaderID, "Uniforms");
	glGetActiveUniformBlockiv(ordinaryShader.m_ShaderID, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
	buffer = malloc(uboSize);
	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate buffer\n");
		exit(EXIT_FAILURE);
	}
	else {
		enum { Translation, Scale, Rotation, Enabled, NumUniforms };

		/* Values to be stored in the buffer object */
		GLfloat scale = 0.5;
		GLfloat translation[] = { 0.1, 0.1, 0.0 };
		GLfloat rotation[] = { 45.0, 0.0, 0.0, 1.0 };
		GLboolean enabled = GL_TRUE;

		/* Since we know the names of the uniforms
		** in our block, make an array of those values */
		const char* names[NumUniforms] = {
		"translation",
		"scale",
		"rotation",
		"enabled"
		};

		/* Query the necessary attributes to determine
		** where in the buffer we should write
		** the values */
		GLuint indices[NumUniforms];
		GLint size[NumUniforms];
		GLint offset[NumUniforms];
		GLint type[NumUniforms];

		glGetUniformIndices(ordinaryShader.m_ShaderID, NumUniforms, names, indices);
		glGetActiveUniformsiv(ordinaryShader.m_ShaderID, NumUniforms, indices, GL_UNIFORM_OFFSET, offset);
		glGetActiveUniformsiv(ordinaryShader.m_ShaderID, NumUniforms, indices, GL_UNIFORM_SIZE, size);
		glGetActiveUniformsiv(ordinaryShader.m_ShaderID, NumUniforms, indices, GL_UNIFORM_TYPE, type);

		/* Copy the uniform values into the buffer */
		memcpy((void*)((intptr_t)buffer + offset[Scale]), &scale,
			size[Scale] * TypeSize(type[Scale]));

		memcpy((void*)((intptr_t)buffer + offset[Translation]), &translation,
			size[Translation] * TypeSize(type[Translation]));

		memcpy((void*)((intptr_t)buffer + offset[Rotation]), &rotation,
			size[Rotation] * TypeSize(type[Rotation]));

		memcpy((void*)((intptr_t)buffer + offset[Enabled]), &enabled,
			size[Enabled] * TypeSize(type[Enabled]));

		/* Create the uniform buffer object, initialize
		** its storage, and associated it with the shader
		** program */
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, uboSize,
			buffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);
	}

	// Unbind everything
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind everything //
		ordinaryShader.use();
		// Put uniforms here, specially if they are varying every frame
		glBindVertexArray(vao);
		//

		// Sending signal to execute OpenGL pipeline with all binded objects //
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		//

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}