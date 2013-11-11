#version 410

// Converts a triangle mesh into a series of lines representing the normals of each vertex
// The vertex positions are assumed to be normalized (i.e. the vertices are assumed to lie on the unit sphere)

layout(triangles, invocations = 3) in;
layout(line_strip, max_vertices = 2) out;

void main()
{
	vec4 position = gl_in[gl_InvocationID].gl_Position;

	gl_Position = position;
	EmitVertex();

	gl_Position = position + 0.1 * vec4(position.xyz, 0);
	EmitVertex();

	EndPrimitive();
}
