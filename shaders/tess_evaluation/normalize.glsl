#version 410

// Normalizes all vertices so that they lie on the unit sphere

layout(quads) in;

uniform mat4 modelview;
uniform mat4 projection;

out EyeSpaceVertex
{
	vec4 position;
	vec4 normal;
} es_Out;

out ObjectSpaceVertex
{
	vec4 position;
	vec4 normal;
} os_Out;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	// Convert the current vertex to cartesian coordinates
	vec3 p0 = (1 - u) * (1 - v) * gl_in[0].gl_Position.xyz;
	vec3 p1 =      u  * (1 - v) * gl_in[1].gl_Position.xyz;
	vec3 p2 =      u  *      v  * gl_in[2].gl_Position.xyz;
	vec3 p3 = (1 - u) *      v  * gl_in[3].gl_Position.xyz;
	vec3 p = normalize(p0 + p1 + p2 + p3);

	os_Out.position = vec4(p, 1.0);
	os_Out.normal = vec4(p, 0.0);

	es_Out.position = modelview * os_Out.position;
	es_Out.normal = modelview * os_Out.normal;

	// Normalize the position of the current vertex so that all vertices sit on the unit sphere
	gl_Position = projection * es_Out.position;
}
