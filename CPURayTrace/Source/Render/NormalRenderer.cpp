#include "NormalRenderer.h"

glm::vec3 NormalRenderer::RenderPixel(const glm::ivec2& PixelCoordinate)
{
	Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate);
	auto HitResultInfo = RenderScene.Intersect(PixelRay);
	if (HitResultInfo.has_value())
	{
		glm::vec3 LinerColor = {0.5f, 0.5f, 0.5f};
		return HitResultInfo->Normal * 0.5f + LinerColor;
	}
	glm::vec3 ZeroLinerColor = {0.f, 0.f, 0.f};
	return ZeroLinerColor;
};