#version 410

// Converts vertex positions to homogeneous coordinates and passes them to OpenGL unmodified

layout(location = 0) in vec3 position;

void main()
{
	gl_Position = vec4(position, 1.0);
}
