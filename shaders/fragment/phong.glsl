#version 410

// Shades fragments using the phong reflection model
// Assumes that the vertex positions are normalized
// TODO: Replace with Blinn-Phong, assume light source is at infinity

uniform mat4 modelview;

in vec4 te_Position;
in vec4 te_Normal;

out vec4 colour;

// Material properties
const vec3  material_ambient  = vec3(0.5, 0.1, 0.1);
const vec3  material_diffuse  = vec3(0.5, 0.2, 0.1);
const vec3  material_specular = vec3(0.7, 0.4, 0.2);
const float shininess         = 5;

// Light properties
const vec4 light_position = vec4(0.0, 0.0, 10.0, 1.0);
const vec3 light_ambient  = vec3(1.0, 1.0,  1.0);
const vec3 light_diffuse  = vec3(1.0, 1.0,  1.0);
const vec3 light_specular = vec3(1.0, 1.0,  1.0);

const vec4 camera_direction = vec4(0.0, 0.0, 1.0, 0.0);

void main()
{
	vec4 light_direction = normalize(light_position - te_Position);
	vec4 light_reflect_direction = normalize(reflect(-light_direction, te_Normal));

	vec3 ambient = material_ambient * light_ambient;
	vec3 diffuse = material_diffuse * max(dot(light_direction, te_Normal), 0.0) * light_diffuse;
	vec3 specular = material_specular * pow(max(dot(light_reflect_direction, camera_direction), 0.0), shininess) * light_specular;

	colour = vec4(ambient + diffuse + specular, 1.0);
}
