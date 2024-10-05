#include "BaseRenderer.h"

#include "ThreadPool.h"
#include "Progress.h"

#include <iostream>

#include "Profile.h"

BaseRenderer::BaseRenderer(Camera& InRenderCamera, const Scene& InRenderScene) :
	RenderCamera(InRenderCamera), RenderScene(InRenderScene), RandHandle(25541, 0.0f, 1.0f)
{

}

void BaseRenderer::Render(size_t InSampleCount, const std::filesystem::path& SavePath)
{
	PROFILE("Render " + std::to_string(InSampleCount) + "InSampleCount " + SavePath.string());

	size_t CurrentSampleCount = 0, IncreaseCount = 1;
	Film& RenderFilm = RenderCamera.GetFilm();
	RenderFilm.ClearFilm();
	Progress RenderProgress(InSampleCount * RenderFilm.GetWidth() * RenderFilm.GetHeight(), 20);
	while (CurrentSampleCount < InSampleCount)
	{
		
		Thread_Pool.ParallelFor(RenderFilm.GetWidth(), RenderFilm.GetHeight(), [&RenderFilm, IncreaseCount, this, &RenderProgress](size_t Wight, size_t Height)
		{
			for (int Index = 0; Index < IncreaseCount; Index++)
			{
				RenderFilm.AddSample(Wight, Height, RenderPixel({ Wight, Height }));
			}
			RenderProgress.UpdatePercent(IncreaseCount);
		});
		Thread_Pool.Wait();
		CurrentSampleCount += IncreaseCount;
		IncreaseCount = glm::min<size_t>(CurrentSampleCount, 32);
		RenderFilm.Save(SavePath);

		std::cout << "CurrentSample : " << CurrentSampleCount << " Save Name: " << SavePath << " ! " << std::endl;
	}	
}
