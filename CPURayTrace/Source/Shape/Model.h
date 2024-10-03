#pragma once
#include "Triangle.h"
#include <filesystem>

class Model : public Shape
{
public:
	Model(const std::vector<Triangle> InTriangles);
	Model(const std::filesystem::path& FilePath);

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min = 1e-5, float T_Max = std::numeric_limits<float>::infinity()) const override;

	bool LoadFile(const std::filesystem::path& FilePath);
private:
	std::vector<Triangle> Triangles;
};
