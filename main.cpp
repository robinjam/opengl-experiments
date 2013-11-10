#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "shader.hpp"
#include "program.hpp"

int main(int argc, const char *argv[])
{
	int result = EXIT_SUCCESS;

	try
	{
		if (!glfwInit())
			throw std::runtime_error("glfwInit failed");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow *window = glfwCreateWindow(1280, 720, "COMP3004", nullptr, nullptr);
		if (!window)
			throw std::runtime_error("glfwCreateWindow failed");

		glfwMakeContextCurrent(window);

		std::cout << "[INFO] GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "[INFO] GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "[INFO] GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "[INFO] GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		shader passthrough("shaders/vertex/passthrough.glsl", GL_VERTEX_SHADER);
		shader wireframe("shaders/geometry/wireframe.glsl", GL_GEOMETRY_SHADER);
		shader solid_white("shaders/fragment/solid_white.glsl", GL_FRAGMENT_SHADER);
		program prog{&passthrough, &wireframe, &solid_white};
		prog.use();

		GLbyte vertices[] = { -1, -1, 0, 0, 1, 0, 1, -1, 0 };
		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (std::exception &ex)
	{
		std::cerr << "[ERROR] " << ex.what() << std::endl;
		result = EXIT_FAILURE;
	}

	glfwTerminate();
	return result;
}
