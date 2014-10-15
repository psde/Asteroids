#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"
#include "Geometry/Mesh.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int main()
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	std::vector<GLfloat> vertices = {
		100.0f, 500.0f,
		500.0f, 500.0f,
		500.0f, 100.0f,
		100.0f, 100.0f
	};

	std::vector<GLuint> elements = {
		0, 1, 2,
		2, 3, 0
	};

	std::unique_ptr<Geometry::Mesh> mesh(new Geometry::Mesh(vertices, elements));

	Shader::Program shader;
	shader.addFragmentShader("data/shader/test.glsl");
	shader.addVertexShader("data/shader/test.glsl");
	
	auto factorUniform = shader.createUniform("factor");
	auto widthUniform = shader.createUniform("width");
	auto heightUniform = shader.createUniform("height");
	
	float factor = 0.0f;
	int tick = 0;
	double time = glfwGetTime();
	double timeDelta = 0;
    while (!glfwWindowShouldClose(window))
	{
		double oldTime = time;
		time = glfwGetTime();
		timeDelta = time - oldTime;

		if (tick % 1000 == 0)
		{
			shader.reload();
		}

		tick++;
		float ratio;
		int width, height;
		
		factor += 1.5f * timeDelta;
		factorUniform.update(fmod(factor, 1.0f));

		glfwGetWindowSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		widthUniform.update(width);
		heightUniform.update(height);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		mesh->setup(&shader);
		mesh->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}