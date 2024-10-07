#include "Plane.h"

Plane::Plane(glm::vec3 InPos, glm::vec3 InNormal) :
	 Pos(InPos), Normal(InNormal)
{

}

// O: Origin D: Direction t:TStep/Time
// N: PlanNormal P: PlanPosition

// (O + tD - P) * N == 0
// t = ((P - O) * N) / (D * N)
// t > 0
// solve function
std::optional<HitInfo> Plane::Intersect(const Ray& InRay, float T_Min /*= 1e-5*/, float T_Max /*= std::numeric_limits<float>::infinity()*/) const
{

	float Hit_TStep = glm::dot((Pos - InRay.Origin), Normal) / glm::dot(InRay.Direction, Normal);
	if (Hit_TStep > T_Min && Hit_TStep < T_Max)
	{
		HitInfo ResultInfo;
		ResultInfo.TStep = Hit_TStep;
		ResultInfo.HitPos = InRay.Hit(Hit_TStep);
		ResultInfo.Normal = Normal;

		return ResultInfo;
	}
	return std::nullopt;
}

Bounds Plane::GetBounds() const
{
	return {};
}
