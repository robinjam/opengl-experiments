#version 410

// Shades fragments using the phong reflection model
// Assumes that the vertex positions are normalized

in vec3 te_Position;

out vec4 frag_colour;

// Material properties
const vec3  material_ambient  = vec3(0.5, 0.1, 0.1);
const vec3  material_diffuse  = vec3(0.5, 0.2, 0.1);
const vec3  material_specular = vec3(0.7, 0.4, 0.2);
const float shininess         = 5;

// Light properties
const vec3 light_position = vec3(-5.0, 5.0, 10.0);
const vec3 light_ambient  = vec3( 1.0, 1.0,  1.0);
const vec3 light_diffuse  = vec3( 1.0, 1.0,  1.0);
const vec3 light_specular = vec3( 1.0, 1.0,  1.0);

void main()
{
	// The position and normal of the current fragment
	vec3 position = te_Position;
	vec3 normal = normalize(te_Position);

	// The direction of the light source from this fragment
	vec3 light_direction = normalize(light_position - position);

	// The direction a photon from the light would bounce off the surface at this position, if the object was perfectly reflective
	vec3 light_reflect_direction = normalize(reflect(-light_direction, normal));

	// Since the camera is at the origin, the direction from this fragment to the camera is normalize(0 - position)
	vec3 camera_direction = normalize(position);

	vec3 ambient = material_ambient * light_ambient;
	vec3 diffuse = material_diffuse * max(dot(light_direction, normal), 0.0) * light_diffuse;
	vec3 specular = material_specular * pow(max(dot(light_reflect_direction, camera_direction), 0.0), shininess) * light_specular;

	frag_colour = vec4(ambient + diffuse + specular, 1.0);
}
