#include "PathTraceRenderer.h"
#include "FrameSpace.h"
#include "ConstantDefine.hpp"
#include "Spherical.h"

//Radiant energy: dQ(J)
//Radiant Flux: W(watt) = dQ(energy) / dt(time) 
//Radiant Intensity: I(candela) = dW(watt) / dw(solid angle)
//Irradiance: E(W/S) =  dW(watt) / dA(area)
//Radiance: L(nit) = dW(watt) * dW(watt) / (dw(solid angle) * dA(area) * cos(angle)

glm::vec3 PathTraceRenderer::RenderPixel(const glm::ivec2& PixelCoordinate)
{
	Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate, { RandHandle.GetRandom(), RandHandle.GetRandom() });
	glm::vec3 Beta = { 1.0f, 1.0f, 1.0f };
	//Radiance
	glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };

	float Probability = 0.9f;
	while (true)
	{
		auto HitResultInfo = RenderScene.Intersect(PixelRay);
		if (HitResultInfo.has_value())
		{

			Radiance += Beta * HitResultInfo->HitMaterial->Emissive;
			
			if (RandHandle.GetRandom() > Probability)
			{
				break;
			}
			Beta *= HitResultInfo->HitMaterial->Albedo;
			Beta /= Probability;
			FrameSpace ReflectRayFrameSpace(HitResultInfo->Normal);
			glm::vec3 LocalRayDirection = {};
			//glm::vec3 BRDF = {};
			//float PDF = 0;
			if (HitResultInfo->HitMaterial->bSpecular)
			{
				glm::vec3 LocalViewDirection = ReflectRayFrameSpace.LocalFromWorld(-PixelRay.Direction);
				LocalRayDirection = { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
				//PDF = 1.0f;
				//BRDF = HitResultInfo->HitMaterial->Albedo / LocalRayDirection.y;
			}
			else
			{
				LocalRayDirection = SpheraicalSample::CosineSampleHemisphere({ RandHandle.GetRandom(), RandHandle.GetRandom() });
				//PDF = 1 / (2PI)
				//BRDF = Albedo / PI
				//float InversePI = 1.0f / static_cast<float>(PI);
				//PDF = InversePI * 0.5f;
				//BRDF = HitResultInfo->HitMaterial->Albedo * InversePI;
				//PDF = LocalRayDirection.y * InversePI;
				//BRDF = HitResultInfo->HitMaterial->Albedo * InversePI;
			}
			
			//Beta *= BRDF * LocalRayDirection.y / PDF;
			LocalRayDirection = glm::normalize(LocalRayDirection);
			PixelRay.Origin = HitResultInfo->HitPos;
			PixelRay.Direction = ReflectRayFrameSpace.WorldFromLocal(LocalRayDirection);
		}
		else
		{
			break;
		}
	}

	return Radiance;
};