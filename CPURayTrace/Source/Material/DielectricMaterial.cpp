#include "DielectricMaterial.h"
#include "MaterialFuction.hpp"

DielectricMaterial::DielectricMaterial(float InIOR, const glm::vec3& InAlbedo)
	: IOR(InIOR), AlbedoReflect(InAlbedo), AlbedoTranslucent(InAlbedo)

{

}

//BSDF = BRDF + BTDF
// Fresnel
// L = Fr * Lr + Ft * Lt
// Snell
// IOR1 * sin(A1) = IOR2 * sin(A2)
// r1 = (IOR2 * cos(A1) - IOR1 * cos(A2)) / (IOR2 * cos(A1) + IOR1 * cos(A2))
// r2 = (IOR1 * cos(A1) - IOR2 * cos(A2)) / (IOR1 * cos(A1) + IOR2 * cos(A2))
// Fr = 0.5 * (r1 * r1 + r2 * r2)
// Ft = 1 - Fr
glm::vec3 DielectricMaterial::SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const
{
	float EtaiDivEtat = IOR;
	glm::vec3 ObjectNormal = {0.f, 1.0f, 0.f};
	float CosThetaT = LocalViewDirection.y;
	glm::vec3 LocalRayDirection = {};
	if (CosThetaT < 0.f)
	{
		EtaiDivEtat = 1.f / IOR;
		ObjectNormal = { 0.f, -1.0f, 0.f };
		CosThetaT = -CosThetaT;
	}
	float CosThetaI = 0.f;
	float Fr = MaterialFunction::Fresnel(EtaiDivEtat, CosThetaT, CosThetaI);
	if (RandHandle.GetRandom() < Fr)
	{
		Beta *= AlbedoReflect;
		LocalRayDirection = { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
	}
	else
	{
		Beta *= AlbedoTranslucent / (EtaiDivEtat * EtaiDivEtat);
		LocalRayDirection = (-LocalViewDirection / EtaiDivEtat) + (CosThetaT / EtaiDivEtat - CosThetaI) * ObjectNormal;
	}
	LocalRayDirection = glm::normalize(LocalRayDirection);
	return LocalRayDirection;
}

