#pragma once
#include "glm/glm.hpp"
#include "RandomDistribution.hpp"

class Material
{
public:
	virtual glm::vec3 Sample_BRDF(const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const = 0;

	void SetEmissive(const glm::vec3& InEmissive);
	glm::vec3 GetEmissive() const;
private:
	glm::vec3 Emissive = { 0.f,0.f ,0.f };
};
