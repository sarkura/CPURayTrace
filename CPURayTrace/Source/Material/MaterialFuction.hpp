#pragma once
#include "glm/glm.hpp"
namespace MaterialFunction
{
	// Fresnel
	// L = Fr * Lr + Ft * Lt
	// Snell
	// IOR1 * sin(I) = IOR2 * sin(T)
	// IOR1 / IOR2 = IOR
	// r1 = (IOR2 * cos(I) - IOR1 * cos(T)) / (IOR2 * cos(I) + IOR1 * cos(T))
	// r2 = (IOR1 * cos(I) - IOR2 * cos(T)) / (IOR1 * cos(I) + IOR2 * cos(T))
	// Fr = 0.5 * (r1 * r1 + r2 * r2)
	// Ft = 1 - Fr

	float Fresnel(float EtaiDivEtat, float CosThetaT, float& CosThetaI)
	{
		float Sin2ThetaT = 1.0f - CosThetaT * CosThetaT;
		float Sin2ThetaI = Sin2ThetaT / (EtaiDivEtat * EtaiDivEtat);

		if (Sin2ThetaI >= 1.0f)
		{
			return 1.0f;
		}
		CosThetaI = glm::sqrt(1.0f - Sin2ThetaI);
		//r1
		float RParallelL = (CosThetaI - EtaiDivEtat * CosThetaT) / (CosThetaI + EtaiDivEtat * CosThetaT);
		//r2
		float RParallelP = (EtaiDivEtat * CosThetaI - CosThetaT) / (EtaiDivEtat * CosThetaI + CosThetaT);
		return 0.5f * (RParallelL * RParallelL + RParallelP * RParallelP);
	}
};
