#pragma once

#include "Ray.h"
#include "Shape.h"

#include <optional>

class Sphere : public Shape
{
public:
	Sphere(const glm::vec3 InCenter, float InRadius);
	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min, float T_Max) const override;
	virtual Bounds GetBounds() const override;
protected:
	glm::vec3 Center;
	float Radius;
};

