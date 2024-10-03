#pragma once

#include "Camera.h"
#include "Scene.h"
#include "RandomDistribution.hpp"

#include <filesystem>

#define DEFINE_RENDER(Name) \
	class Name##Renderer : public BaseRenderer \
	{ \
	public: \
		Name##Renderer(Camera& InRenderCamera, const Scene& InRenderScene) : BaseRenderer(InRenderCamera, InRenderScene){} \
	private: \
		virtual glm::vec3 RenderPixel(const glm::ivec2& PixelCoordinate) override; \
	}; \


class BaseRenderer
{
public:

	BaseRenderer(Camera& InRenderCamera, const Scene& InRenderScene);

	void Render(size_t InSampleCount, const std::filesystem::path& SavePath);

private:
	virtual glm::vec3 RenderPixel(const glm::ivec2& PixelCoordinate) = 0;
protected:
	Camera& RenderCamera;
	const Scene& RenderScene;
	RandomDistribution<float> RandHandle;
};
