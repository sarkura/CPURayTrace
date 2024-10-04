#include "Film.h"
#include "RGB.h"

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
	PROFILE(Film_Save);
	//PPM
	std::ofstream File(FileName,std::ios::binary);
	File << "P6\n" << Width << ' ' << Height << "\n255\n";
	for (size_t Y = 0; Y < Height; Y++)
	{
		for (size_t X = 0; X < Width; X++)
		{
			const Pixel& PixelValue = GetPixel(X, Y);
			RGB Color(PixelValue.LinerColor / static_cast<float>(PixelValue.SampleCount));
			File << static_cast<uint8_t>(Color.GetRedChannel())
				 << static_cast<uint8_t>(Color.GetGreenChannel())
				 << static_cast<uint8_t>(Color.GetBlueChannel());
		}
	}
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

