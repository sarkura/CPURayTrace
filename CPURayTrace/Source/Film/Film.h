#pragma once

#include <filesystem>
#include <vector>
#include "glm/glm.hpp"

struct Pixel
{
	glm::vec3 LinerColor = {0.f, 0.f, 0.f};
	int SampleCount = 0;
};

class Film
{
public:

	Film(size_t InWidth, size_t InHeight);
	void Save(const std::filesystem::path& FileName);

	inline size_t GetWidth() const { return Width; }
	inline size_t GetHeight() const { return Height; }
	
	Pixel GetPixel(size_t X, size_t Y);
	void AddSample(size_t X, size_t Y, const glm::vec3& LinerColor);

	void ClearFilm();
private:
	std::vector<Pixel> Pixels;
	size_t Width;
	size_t Height;
	const size_t MaxSize = 4096;
};