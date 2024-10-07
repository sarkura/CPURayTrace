#pragma once

#include "Ray.h"
#include "Bounds.h"
#include <optional>

class Shape
{
public:
	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min, float T_Max) const = 0;
	virtual Bounds GetBounds() const { return {}; }
};
