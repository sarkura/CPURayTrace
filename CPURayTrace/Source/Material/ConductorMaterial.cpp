#include "ConductorMaterial.h"
#include "Complex.hpp"

ConductorMaterial::ConductorMaterial(glm::vec3 InIOR, const glm::vec3 InK)
	: IOR(InIOR), K(InK)
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
glm::vec3 ConductorMaterial::SampleBSDF(const glm::vec3& HitPostion, const glm::vec3& LocalViewDirection, glm::vec3& Beta, const RandomDistribution<float>& RandHandle) const
{
	glm::vec3 Fr = {};
	for (int i = 0; i < 3; i++)
	{
		Complex EtatDivEtai{ IOR[i], K[i] };
		float CosThetaI = glm::clamp(LocalViewDirection.y, 0.f, 1.f);
		float Sin2ThetaI = 1.f - CosThetaI * CosThetaI;
		Complex Sin2ThetaT = Sin2ThetaI / (EtatDivEtai * EtatDivEtai);
		Complex CosThetaT = Sqrt(1.f - Sin2ThetaT);

		Complex RParallelL = (EtatDivEtai * CosThetaI - CosThetaT) / (EtatDivEtai * CosThetaI + CosThetaT);
		Complex RParallelP = (CosThetaI - EtatDivEtai * CosThetaT) / (CosThetaI + EtatDivEtai * CosThetaT);

		Fr[i] = 0.5f * (Normalize(RParallelL) + Normalize(RParallelP));
	}
	Beta *= Fr;
	return { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
}
