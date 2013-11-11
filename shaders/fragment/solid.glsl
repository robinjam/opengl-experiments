#version 410

// Shades every fragment the same colour

uniform vec4 colour;

out vec4 frag_colour;

void main()
{
	frag_colour = colour;
}
