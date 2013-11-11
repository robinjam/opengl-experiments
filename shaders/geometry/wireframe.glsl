#version 410

// Converts a triangle mesh into a wireframe by replacing each triangle with a line loop

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	EndPrimitive();
}
