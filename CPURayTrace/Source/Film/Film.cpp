#include "Film.h"
#include "RGB.h"
#include "ThreadPool.h"

#include <iostream>
#include <fstream>
#include "Profile.h"


Film::Film(size_t InWidth, size_t InHeight)
{
	Width = std::min(MaxSize, InWidth);
	Height = std::min(MaxSize, InHeight);
	Pixels.resize(Width * Height);
}

void Film::Save(const std::filesystem::path& FileName)
{
	PROFILE("Filmsave");
	size_t& CaptureWidth = Width;
	size_t& CaptureHeight = Height;

	std::vector<uint8_t> ColorBuffer(Width * Height * 3);

	Thread_Pool.ParallelFor(Width, Height, [this, CaptureWidth, CaptureHeight, &ColorBuffer](size_t X, size_t Y)
	{
		const Pixel& PixelValue = GetPixel(X, Y);
		RGB Color(PixelValue.LinerColor / static_cast<float>(PixelValue.SampleCount));
		size_t ColorStart = (Y * Width + X) * 3;
		ColorBuffer[ColorStart + 0] = Color.GetRedChannel();
		ColorBuffer[ColorStart + 1] = Color.GetGreenChannel();
		ColorBuffer[ColorStart + 2] = Color.GetBlueChannel();
	}, false);
	Thread_Pool.Wait();
	//PPM
	std::ofstream File(FileName,std::ios::binary);
	File << "P6\n" << Width << ' ' << Height << "\n255\n";
	File.write(reinterpret_cast<const char*>(ColorBuffer.data()), ColorBuffer.size());
}

Pixel Film::GetPixel(size_t X, size_t Y)
{
	if (X < Width && Y < Height)
		return Pixels[Y * Width + X];
	return Pixel();
}

void Film::AddSample(size_t X, size_t Y, const glm::vec3& LinerColor)
{
	if (X < Width && Y < Height)
	{
		Pixels[Y * Width + X].LinerColor += LinerColor;
		Pixels[Y * Width + X].SampleCount += 1;
	}
	else
		std::cout << "Pixel location Error!" << " X :" << X << " Y :" << Y << std::endl;
}

void Film::ClearFilm()
{
	Pixels.clear();
	Pixels.resize(Width * Height);;
}

