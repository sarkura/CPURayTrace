#include "GroundMaterial.h"
#include "Spherical.h"

glm::vec3 GroundMaterial::SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const
{
	Beta *= Albedo;
	if (
		(static_cast<int>(glm::floor(HitPostion.x * 8)) % 8 == 0) ||
		(static_cast<int>(glm::floor(HitPostion.z * 8)) % 8 == 0)
		) 
	{
		Beta *= 0.1f;
	}
	return SpheraicalSample::CosineSampleHemisphere({ RandHandle.GetRandom(), RandHandle.GetRandom() });
}
