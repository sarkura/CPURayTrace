#pragma once

#include "glm/glm.hpp"
#include "Ray.h"

class Bounds
{
public:
	Bounds();
	Bounds(const glm::vec3& InBoundsMin, const glm::vec3& InBoundsMax);
	void Expand(const glm::vec3& InPos);
	void Expand(const Bounds& InBounds);
	bool HasIntersection(const Ray& InRay, float T_Min, float T_Max) const;
	bool HasIntersection(const Ray& InRay, const glm::vec3& InverseRayDirection, float T_Min, float T_Max) const;
	void InvaildBounds();
	glm::vec3 GetBoundsDiagonal() const;
	float Area() const;
	glm::vec3 GetCorner(size_t Idx) const;
	bool IsValid() const;
public:
	glm::vec3 BoundsMin;
	glm::vec3 BoundsMax;
};