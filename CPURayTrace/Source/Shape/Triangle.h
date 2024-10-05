#pragma once

#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(const glm::vec3 InVertexPos0, const glm::vec3 InVertexPos1, const glm::vec3 InVertexPos2,
		const glm::vec3 InNormal0, const glm::vec3 InNormal1, const glm::vec3 InNormal2);

	Triangle(const glm::vec3 InVertexPos0, const glm::vec3 InVertexPos1, const glm::vec3 InVertexPos2);

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min = 1e-5, float T_Max = std::numeric_limits<float>::infinity()) const override;
public:
	glm::vec3 VertexPos0, VertexPos1, VertexPos2;
	glm::vec3 Normal0, Normal1, Normal2;
};

