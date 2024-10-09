#pragma once
#include "Material.h"
class DielectricMaterial : public Material
{
public:
	DielectricMaterial(float InIOR = 1.0f, const glm::vec3& InAlbedo = { 0.0f, 0.0f, 0.0f });
	virtual glm::vec3 SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const override;
public:
	glm::vec3 AlbedoReflect, AlbedoTranslucent;
	float IOR;
};