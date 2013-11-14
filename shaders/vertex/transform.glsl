#version 410

// Transforms the input vertices using the given matrices

uniform mat4 modelview;
uniform mat4 projection;

layout(location = 0) in vec3 position;

out vec4 te_Position;

void main()
{
	te_Position = modelview * vec4(position, 1.0);
	gl_Position = projection * te_Position;
}
