#pragma once
#include "Material.h"
class SpecularMaterial : public Material
{
public:
	SpecularMaterial(const glm::vec3& InAlbedo = { 1.0f, 1.0f, 1.0f });
	virtual glm::vec3 Sample_BRDF(const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const override;
public:
	glm::vec3 Albedo;
};