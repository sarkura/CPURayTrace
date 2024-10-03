#pragma once
#include "glm/glm.hpp"

class Material
{
public:
	glm::vec3 Albedo = {1.0f, 1.0f, 1.0f};
	bool bSpecular = false;
	glm::vec3 Emissive = { 0.0f, 0.0f, 0.0f };
};

extern const Material ErrorMaterial;