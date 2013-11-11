#version 410

// Normalizes all vertices so that they lie on the unit sphere

layout(quads) in;

uniform mat4 transform;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	// Convert the current vertex to cartesian coordinates
	vec3 p0 = (1 - u) * (1 - v) * gl_in[0].gl_Position.xyz;
	vec3 p1 =      u  * (1 - v) * gl_in[1].gl_Position.xyz;
	vec3 p2 =      u  *      v  * gl_in[2].gl_Position.xyz;
	vec3 p3 = (1 - u) *      v  * gl_in[3].gl_Position.xyz;
	vec3 p = p0 + p1 + p2 + p3;

	// Normalize the position of the current vertex so that all vertices sit on the unit sphere
	gl_Position = transform * vec4(normalize(p), 1.0);
}
