#pragma once

#include "Material.h"

class GroundMaterial : public Material 
{
public:
	GroundMaterial(const glm::vec3& InAlbedo) : Albedo(InAlbedo) {}
	glm::vec3 SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const override;
private:
	glm::vec3 Albedo{};
};