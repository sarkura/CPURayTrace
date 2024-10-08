#include "DiffuseMaterial.h"
#include "Spherical.h"

DiffuseMaterial::DiffuseMaterial(const glm::vec3& InAlbedo)
	: Albedo(InAlbedo)
{

}

//PDF = 1 / (2PI)
//BRDF = Albedo / PI
//float InversePI = 1.0f / static_cast<float>(PI);
//PDF = InversePI * 0.5f;


//BRDF = Albedo * InversePI;
//PDF = LocalRayDirection.y * InversePI;
glm::vec3 DiffuseMaterial::Sample_BRDF(const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const
{
	Beta *= Albedo;
	glm::vec3 LocalRayDirection = SpheraicalSample::CosineSampleHemisphere({ RandHandle.GetRandom(), RandHandle.GetRandom() });
	return LocalRayDirection;
}

