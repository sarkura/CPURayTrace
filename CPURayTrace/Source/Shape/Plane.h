#pragma once
#include "Shape.h"

class Plane : public Shape
{
public:
	Plane(glm::vec3 InPos, glm::vec3 InNormal);

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min = 1e-5, float T_Max = std::numeric_limits<float>::infinity()) const override;

private:
	glm::vec3 Normal;
	glm::vec3 Pos;
};

