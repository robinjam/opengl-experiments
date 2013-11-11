#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

#include "shader.hpp"
#include "program.hpp"

namespace
{
	unsigned char num_subdivisions = 8;

	void update_subdivisions()
	{
		GLfloat n = GLfloat(num_subdivisions) + 1.f;
		GLfloat outer[] = { n, n, n, n };
		GLfloat inner[] = { n, n };
		glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outer);
		glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, inner);
	}

	void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			if (key == GLFW_KEY_EQUAL && mods & GLFW_MOD_SHIFT)
			{
				num_subdivisions++;
				update_subdivisions();
			}
			if (key == GLFW_KEY_MINUS)
			{
				num_subdivisions--;
				update_subdivisions();
			}
		}
	}
}

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

		glfwSetKeyCallback(window, key_callback);

		std::cout << "[INFO] GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "[INFO] GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "[INFO] GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "[INFO] GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		shader passthrough("shaders/vertex/passthrough.glsl", GL_VERTEX_SHADER);
		shader quadsphere("shaders/tess_evaluation/normalize.glsl", GL_TESS_EVALUATION_SHADER);
		shader wireframe("shaders/geometry/wireframe.glsl", GL_GEOMETRY_SHADER);
		shader solid_white("shaders/fragment/solid_white.glsl", GL_FRAGMENT_SHADER);
		program prog{&passthrough, &quadsphere, &wireframe, &solid_white};
		prog.use();

		GLfloat a = 1 / sqrt(3);
		GLfloat vertices[] = {
			 a,  a,  a,
			-a,  a,  a,
			-a, -a,  a,
			 a, -a,  a,
			 a, -a, -a,
			 a,  a, -a,
			-a,  a, -a,
			-a, -a, -a
		};
		GLubyte indices[] = {
			0, 1, 2, 3,
			0, 3, 4, 5,
			0, 5, 6, 1,
			1, 6, 7, 2,
			7, 4, 3, 2,
			4, 7, 6, 5
		};

		GLuint vao, vbo, ibo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glPatchParameteri(GL_PATCH_VERTICES, 4);

		glm::mat4 projection = glm::perspective(45.f, 1280.f / 720.f, 1.f, 10.f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.f, 1.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 model;

		update_subdivisions();

		while (!glfwWindowShouldClose(window))
		{
			model = glm::rotate(model, float(glfwGetTime() * 10), glm::vec3(0.f, 1.f, 0.f));
			glfwSetTime(0.);
			glm::mat4 mvp = projection * view * model;
			prog.set_uniform("transform", mvp);

			glClear(GL_COLOR_BUFFER_BIT);
			glDrawElements(GL_PATCHES, 24, GL_UNSIGNED_BYTE, nullptr);

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
