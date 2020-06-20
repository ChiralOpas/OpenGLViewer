#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


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

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indicies, GL_STATIC_DRAW);

	// 4x3 aspect ration
	glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);



	Shader ordinaryShader("../res/vertex.glsl", "../res/fragment.glsl");
	int uniform = glGetUniformLocation(ordinaryShader.m_ShaderID, "u_mvp");

	// getting location of subroutines
	GLuint rbSelection = glGetSubroutineUniformLocation(ordinaryShader.m_ShaderID, GL_VERTEX_SHADER, "theRedBlueSelection");
	GLuint redIndex = glGetSubroutineIndex(ordinaryShader.m_ShaderID, GL_VERTEX_SHADER, "redColor");
	GLuint blueIndex = glGetSubroutineIndex(ordinaryShader.m_ShaderID, GL_VERTEX_SHADER, "blueColor");

	ordinaryShader.use();
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &projection[0][0]);

	//

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
		
		// have to bind subroutine again as when we unbind shader program ,as the active subroutine goes to default one
		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &blueIndex);
		
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