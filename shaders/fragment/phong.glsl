#version 410

// Shades fragments using the phong reflection model

in EyeSpaceVertex
{
	vec4 position;
	vec4 normal;
} es_In;

out vec4 colour;

// Material properties
const vec3  material_ambient  = vec3(0.05, 0.01, 0.01);
const vec3  material_diffuse  = vec3(0.75, 0.15, 0.15);
const vec3  material_specular = vec3(0.5 , 0.3 , 0.2);
const float shininess         = 10;

// Light properties
vec4 light_direction      = vec4(0.0, 0.0, 1.0, 0.0);
const vec3 light_ambient  = vec3(1.0, 1.0, 1.0);
const vec3 light_diffuse  = vec3(1.0, 1.0, 1.0);
const vec3 light_specular = vec3(1.0, 1.0, 1.0);

const vec4 camera_direction = vec4(0.0, 0.0, 1.0, 0.0);

void main()
{
	vec4 light_reflect_direction = normalize(reflect(-light_direction, es_In.normal));

	vec3 ambient = material_ambient * light_ambient;
	vec3 diffuse = material_diffuse * max(dot(light_direction, es_In.normal), 0.0) * light_diffuse;
	vec3 specular = material_specular * pow(max(dot(light_reflect_direction, camera_direction), 0.0), shininess) * light_specular;

	colour = vec4(ambient + diffuse + specular, 1.0);
}
