#include "Material.h"

void Material::SetEmissive(const glm::vec3& InEmissive)
{
	Emissive = InEmissive;
}

glm::vec3 Material::GetEmissive() const
{
	return Emissive;
}
