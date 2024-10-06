#include "Bounds.h"

Bounds::Bounds(const glm::vec3& InBoundsMin, const glm::vec3& InBoundsMax):
	BoundsMin(InBoundsMin), BoundsMax(InBoundsMax)
{

}

Bounds::Bounds():
	BoundsMin(std::numeric_limits<float>::infinity()), BoundsMax(-std::numeric_limits<float>::infinity())
{

}

void Bounds::Expand(const glm::vec3& InPos)
{
	BoundsMin = glm::min(InPos, BoundsMin);
	BoundsMax = glm::max(InPos, BoundsMax);
}

void Bounds::Expand(const Bounds& InBounds)
{
	BoundsMin = glm::min(InBounds.BoundsMin, BoundsMin);
	BoundsMax = glm::max(InBounds.BoundsMax, BoundsMax);
}

// O: Origin D: Direction t:TStep/Time
// N: PlanNormal P: PlanPosition
// N = {1, 0, 0}, {0, 1, 0}, {0, 0, 1}
// FaceY = Y
// (O + tD - P) * N == 0
// t = ((P - O) * N) / (D * N)
// t = (Y - O.y) / (D.y)
// solve function

// ????
bool Bounds::HasIntersection(const Ray& InRay, float T_Min, float T_Max) const
{
	/*glm::vec3 t1 = (BoundsMin - InRay.Origin) / InRay.Direction;
	glm::vec3 t2 = (BoundsMax - InRay.Origin) / InRay.Direction;
	glm::vec3 tmin = glm::min(t1, t2);
	glm::vec3 tmax = glm::max(t1, t2);

	float near = glm::max(tmin.x, glm::max(tmin.y, tmin.z));
	float far = glm::min(tmax.x, glm::min(tmax.y, tmax.z));

	if (near <= T_Min && far >= T_Max) {
		return false;
	}*/

	float TEntry = T_Min;
	float TExit = T_Max;

	for (int i = 0; i < 3; ++i) 
	{
		if (InRay.Direction.x != 0) 
		{
			float tMin = (BoundsMin.x - InRay.Origin.x) / InRay.Direction.x;
			float tMax = (BoundsMax.x - InRay.Origin.x) / InRay.Direction.x;

			if (tMin > tMax) std::swap(tMin, tMax);

			TEntry = std::max(TEntry, tMin);
			TExit = std::min(TExit, tMax);
		}
		else if (InRay.Origin.x < BoundsMin.x || InRay.Origin.x > BoundsMax.x)
		{
			return false;
		}
	}

	return TEntry <= TExit;
}

void Bounds::InvaildBounds()
{
	BoundsMin = { std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() , std::numeric_limits<float>::infinity() };
	BoundsMax = { -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() , -std::numeric_limits<float>::infinity() };
}

glm::vec3 Bounds::GetBoundsDiagonal() const
{
	return BoundsMax - BoundsMin;
}
