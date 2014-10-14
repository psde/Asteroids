#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"
#include "Geometry/Mesh.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void setupShader(Shader::Program shader)
{
	//glBindFragDataLocation(shader.getProgram(), 0, "outColor");

	GLint posAttrib = glGetAttribLocation(shader.getProgram(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	//GLint colAttrib = glGetAttribLocation(shader.getProgram(), "color");
	//glEnableVertexAttribArray(colAttrib);
	//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

int main()
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	std::vector<GLfloat> vertices__ = { 
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	std::vector<GLfloat> vertices = {
		100.0f, 500.0f,
		500.0f, 500.0f,
		500.0f, 100.0f,
		100.0f, 100.0f
	};

	std::vector<GLfloat> vertices11 = {
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f
	};

	std::vector<GLuint> elements = {
		0, 1, 2,
		2, 3, 0
	};

	auto *mesh = new Geometry::Mesh(vertices, elements);

	Shader::Program shader;
	shader.addFragmentShader("data/shader/test.glsl");
	shader.addVertexShader("data/shader/test.glsl");

	glBindVertexArray(mesh->id());
	setupShader(shader);
	glBindVertexArray(0);

	glm::mat4 o = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);


	auto factorUniform = shader.createUniform<float>("factor");
	auto widthUniform = shader.createUniform<float>("width");
	auto heightUniform = shader.createUniform<float>("height");
	auto matUniform = shader.createUniform<glm::mat4>("mat");
	
	float factor = 0.0f;
	int tick = 0;
	double time = glfwGetTime();
	double timeDelta = 0;
    while (!glfwWindowShouldClose(window))
    {
		if (tick % 1000 == 0 && shader.reload())
		{
			glBindVertexArray(mesh->id());
			setupShader(shader);
			glBindVertexArray(0);
		}

		tick++;
		float ratio;
		int width, height;
		
		factor += 1.5 * timeDelta;
		factorUniform = fmod(factor, 1.0f);

		glfwGetWindowSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		//widthUniform = width;
		//heightUniform = height;

		matUniform = o;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		mesh->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

		double oldTime = time;
		time = glfwGetTime();
		timeDelta = time - oldTime;
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}