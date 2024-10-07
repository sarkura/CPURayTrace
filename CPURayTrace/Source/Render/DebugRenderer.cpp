#include "DebugRenderer.h"
#include "RGB.h"

glm::vec3 BoundsTestCountRenderer::RenderPixel(const glm::ivec2& PixelCoordinate) 
{
#ifdef WITH_DEBUG_INFO
    Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate);
    RenderScene.Intersect(PixelRay);
    return RGB::GenerateHeatmapRGB(PixelRay.BoundsTestCount / 150.f);
#else
    return {};
#endif
}

glm::vec3 TriangleTestCountRenderer::RenderPixel(const glm::ivec2& PixelCoordinate) 
{
#ifdef WITH_DEBUG_INFO
    Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate);
    RenderScene.Intersect(PixelRay);
    return RGB::GenerateHeatmapRGB(PixelRay.TriangleTestCount / 7.f);
#else
    return {};
#endif
}
