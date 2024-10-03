#include "SimpleRayTraceRenderer.h"
#include "FrameSpace.h"

glm::vec3 SimpleRayTraceRenderer::RenderPixel(const glm::ivec2& PixelCoordinate)
{
	Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate, { RandHandle.GetRandom(), RandHandle.GetRandom() });

	glm::vec3 Reflection = { 1.0f, 1.0f, 1.0f };
	glm::vec3 FinalLinerColor = { 0.0f, 0.0f, 0.0f };

	while (true)
	{
		auto HitResultInfo = RenderScene.Intersect(PixelRay, static_cast<float>(1e-5), std::numeric_limits<float>::infinity());
		if (HitResultInfo.has_value())
		{
			const Material* UsedMaterial = HitResultInfo->HitMaterial;
			if (!HitResultInfo->HitMaterial)
			{
				UsedMaterial = &ErrorMaterial;
			}

			FinalLinerColor += Reflection * UsedMaterial->Emissive;
			Reflection *= UsedMaterial->Albedo;

			PixelRay.Origin = HitResultInfo->HitPos;

			FrameSpace ReflectRayFrameSpace(HitResultInfo->Normal);

			glm::vec3 LocalRayDirection;
			if (UsedMaterial->bSpecular)
			{
				glm::vec3 LocalViewDirection = ReflectRayFrameSpace.LocalFromWorld(-PixelRay.Direction);
				LocalRayDirection = { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
			}
			else
			{
				do
				{
					LocalRayDirection = { RandHandle.GetRandom(), RandHandle.GetRandom(), RandHandle.GetRandom() };
					LocalRayDirection = LocalRayDirection * 2.f - 1.0f;
				} while (glm::length(LocalRayDirection) > 1.0f);

				if (LocalRayDirection.y < 0.f)
				{
					LocalRayDirection.y = -LocalRayDirection.y;
				}
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