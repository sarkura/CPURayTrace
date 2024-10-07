#include "DebugRenderer.h"
#include "RGB.h"

glm::vec3 BoundsTestCountRenderer::RenderPixel(const glm::ivec2& PixelCoordinate) 
{
    #ifdef WITH_DEBUG_INFO
    Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate);
    auto HitResultInfo = RenderScene.Intersect(PixelRay);
    if (HitResultInfo.has_value())
    {
        return RGB::GenerateHeatmapRGB(HitResultInfo->BoundsTestCount / 200.f);
    }
    
    return {};
    #else
    return {};
    #endif
}

glm::vec3 TriangleTestCountRenderer::RenderPixel(const glm::ivec2& PixelCoordinate) {
    #ifdef WITH_DEBUG_INFO
    Ray PixelRay = RenderCamera.GenerateRayDirection(PixelCoordinate);
    auto HitResultInfo = RenderScene.Intersect(PixelRay);
    if (HitResultInfo.has_value())
    {
        return RGB::GenerateHeatmapRGB(HitResultInfo->TriangleTestCount / 20.f);
    }
    
    return {};
    #else
    return {};
    #endif
}
