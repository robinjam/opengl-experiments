#include <cmath>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

#include "cone.hpp"
#include "cube.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "program.hpp"

namespace
{
	unsigned char num_subdivisions = 7;
	unsigned char mode = 0;

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
			if (key == GLFW_KEY_MINUS && !mods)
			{
				num_subdivisions--;
				update_subdivisions();
			}
			if (key == GLFW_KEY_A)
			{
				mode = 0;
			}
			if (key == GLFW_KEY_B)
			{
				mode = 1;
			}
			if (key == GLFW_KEY_C)
			{
				mode = 2;
			}
			if (key == GLFW_KEY_D)
			{
				mode = 3;
			}
			if (key == GLFW_KEY_E)
			{
				mode = 4;
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
		glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow *window = glfwCreateWindow(1024, 576, "COMP3004", nullptr, nullptr);
		if (!window)
			throw std::runtime_error("glfwCreateWindow failed");

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
			throw std::runtime_error(std::string("glewInit failed with error: ") + (const char *) glewGetErrorString(err));

		// GLEW causes an OpenGL error inside glewInit(), so call glGetError() until all error flags are cleared
		while (glGetError());

		glfwSetKeyCallback(window, key_callback);

		std::cout << "[INFO] GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "[INFO] GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "[INFO] GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "[INFO] GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB);

		glClearColor(1.0, 1.0, 1.0, 1.0);

		GLint value;
		glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &value);
		if (value != GL_SRGB)
			std::cerr << "[WARNING] Default framebuffer is not sRGB capable" << std::endl;

		shader passthrough("shaders/vertex/passthrough.glsl", GL_VERTEX_SHADER);
		shader transform("shaders/vertex/transform.glsl", GL_VERTEX_SHADER);
		shader quadsphere("shaders/tess_evaluation/normalize.glsl", GL_TESS_EVALUATION_SHADER);
		shader wireframe("shaders/geometry/wireframe.glsl", GL_GEOMETRY_SHADER);
		shader normals("shaders/geometry/normals.glsl", GL_GEOMETRY_SHADER);
		shader solid("shaders/fragment/solid.glsl", GL_FRAGMENT_SHADER);
		shader phong("shaders/fragment/phong.glsl", GL_FRAGMENT_SHADER);

		program prog{&passthrough, &quadsphere, &wireframe, &solid};
		prog.set_uniform("colour", glm::vec4(0.f, 0.f, 0.f, 1.f));
		program prog2{&passthrough, &quadsphere, &normals, &solid};
		prog2.set_uniform("colour", glm::vec4(0.f, .5f, 0.f, 1.f));
		program prog3{&passthrough, &quadsphere, &phong};
		program prog4{&transform, &wireframe, &solid};
		prog4.set_uniform("colour", glm::vec4(0.f, 0.f, 0.f, 1.f));

		cube c;
		cone cn(16);

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glm::mat4 projection = glm::perspective(45.f, float(width) / float(height), 1.f, 10.f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.f, 1.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 model;

		update_subdivisions();

		while (!glfwWindowShouldClose(window))
		{
			model = glm::mat4(1.f);
			glm::mat4 modelview = view * model;
			prog.set_uniform("modelview", modelview);
			prog2.set_uniform("modelview", modelview);
			prog3.set_uniform("modelview", modelview);
			prog4.set_uniform("modelview", modelview);
			prog.set_uniform("projection", projection);
			prog2.set_uniform("projection", projection);
			prog3.set_uniform("projection", projection);
			prog4.set_uniform("projection", projection);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch(mode)
			{
			case 2: // Wireframe sphere and normals
				prog2.use();
				c.draw();
			case 0: // Wireframe sphere
				prog.use();
				c.draw();
				break;
			case 1: // Wireframe cone
				prog4.use();
				cn.draw();
				break;
			case 3: // Lit sphere
				prog3.use();
				c.draw();
				break;
			case 4: // Multiple objects moving around
				glm::mat4 tmp = glm::rotate(glm::mat4(1.f), float(glfwGetTime() * 20), glm::vec3(0.f, 1.f, 0.f));

				model = glm::translate(tmp, glm::vec3(-1.5f, cos(glfwGetTime()), 0.f));
				model = glm::rotate(model, float(glfwGetTime() * 45), glm::vec3(0.f, 1.f, 0.f));
				modelview = view * model;
				prog4.set_uniform("modelview", modelview);
				prog4.use();
				cn.draw();

				model = glm::translate(tmp, glm::vec3(1.5f, -cos(glfwGetTime()), 0.f));
				model = glm::rotate(model, float(glfwGetTime() * -45), glm::vec3(0.f, 1.f, 0.f));
				modelview = view * model;
				prog.set_uniform("modelview", modelview);
				prog.use();
				c.draw();
			}

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
