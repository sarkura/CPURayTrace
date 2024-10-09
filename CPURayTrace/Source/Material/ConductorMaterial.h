#pragma once
#include "Material.h"
class ConductorMaterial : public Material
{
public:
	ConductorMaterial(glm::vec3 InIOR = { 1.0f, 1.0f, 1.0f }, const glm::vec3 InK = { 1.0f, 1.0f, 1.0f });
	virtual glm::vec3 SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const override;
public:
	glm::vec3 K;
	glm::vec3 IOR;
};