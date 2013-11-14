#pragma once

#include "mesh.hpp"

class cone : public mesh
{
public:
	cone(unsigned int base_vertices);

	void draw();

private:
	unsigned int num_indices;
};
