#include "SpecularMaterial.h"

SpecularMaterial::SpecularMaterial(const glm::vec3& InAlbedo /*= { 1.0f, 1.0f, 1.0f }*/)
	: Albedo(InAlbedo)
{

}

//PDF = 1.0f;
//BRDF = Albedo / LocalRayDirection.y;

//BRDF = Albedo * InversePI;
//PDF = LocalRayDirection.y * InversePI;
glm::vec3 SpecularMaterial::SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const
{
	Beta *= Albedo;
	glm::vec3 LocalRayDirection = LocalRayDirection = { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
	return LocalRayDirection;
}

