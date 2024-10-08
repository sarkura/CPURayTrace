#include "SimpleRayTraceRenderer.h"
#include "FrameSpace.h"
#include "Spherical.h"

glm::vec3 SimpleRayTraceRenderer::RenderPixel(const glm::ivec2& PixelCoordinate)
{
	Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate, { RandHandle.GetRandom(), RandHandle.GetRandom() });

	glm::vec3 Reflection = { 1.0f, 1.0f, 1.0f };
	glm::vec3 FinalLinerColor = { 0.0f, 0.0f, 0.0f };

	size_t MaxBounceCount = 32;

	while (MaxBounceCount--)
	{
		auto HitResultInfo = RenderScene.Intersect(PixelRay, static_cast<float>(1e-5), std::numeric_limits<float>::infinity());
		if (HitResultInfo.has_value())
		{
			FinalLinerColor += Reflection * HitResultInfo->HitMaterial->Emissive;
			Reflection *= HitResultInfo->HitMaterial->Albedo;

			PixelRay.Origin = HitResultInfo->HitPos;

			FrameSpace ReflectRayFrameSpace(HitResultInfo->Normal);

			glm::vec3 LocalRayDirection;
			if (HitResultInfo->HitMaterial->bSpecular)
			{
				glm::vec3 LocalViewDirection = ReflectRayFrameSpace.LocalFromWorld(-PixelRay.Direction);
				LocalRayDirection = { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
				LocalRayDirection = glm::normalize(LocalRayDirection);
			}
			else
			{
				LocalRayDirection = SpheraicalSample::UniformSampleHemisphere<float>(RandHandle);
			}
			PixelRay.Direction = ReflectRayFrameSpace.WorldFromLocal(LocalRayDirection);
		}
		else
		{
			break;
		}
	}

	return FinalLinerColor;
};