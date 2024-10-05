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

bool Bounds::HasIntersection(const Ray& InRay, float T_Min, float T_Max) const
{
	glm::vec3 T_BoundsMin = (BoundsMin - InRay.Origin) / InRay.Direction;
	glm::vec3 T_BoundsMax = (BoundsMax - InRay.Origin) / InRay.Direction;
	glm::vec3 NewT_BoundsMin = glm::min(T_BoundsMin, T_BoundsMax);
	glm::vec3 NewT_BoundsMax = glm::max(T_BoundsMin, T_BoundsMax);

	float Near = glm::max(T_Min, glm::max(NewT_BoundsMin.x, glm::max(NewT_BoundsMin.y, NewT_BoundsMin.z)));
	float Far = glm::min(T_Max, glm::min(NewT_BoundsMax.x, glm::min(NewT_BoundsMax.y, NewT_BoundsMax.z)));

	return glm::max(Near, T_Min) <= glm::min(Near, T_Max);
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
