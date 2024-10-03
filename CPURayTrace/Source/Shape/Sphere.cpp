#include "Sphere.h"

Sphere::Sphere(const glm::vec3 InCenter, float InRadius) :
	Center(InCenter), Radius(InRadius)
{

}

// O: Origin D: Direction t:TStep/Time
// C: Center R: Radius

// O + tD - C == R
// (O + tD - C) * (O + tD - C) = R * R
// t * t(D * D) + 2t(D * (O - C)) + ((O - C) * (O - C) - R * R) = 0
// D * D = A
// B = D * (O - C)
// C = ((O - C)*(O - C) - R * R)
// A * t * t - 2 * B * t + C = 0
// Solve Function 
// DeltaDelta = B * B - 4 * A * C
std::optional<HitInfo> Sphere::Intersect(const Ray& InRay, float T_Min, float T_Max) const
{
	glm::vec3 Dir = InRay.Origin - Center;
	float A = glm::dot(InRay.Direction, InRay.Direction);
	float B = 2.0f * glm::dot(InRay.Direction, Dir);
	float C = glm::dot(Dir, Dir) - Radius * Radius;
	float Delta = B * B - 4 * A * C;
	float InverseA = 1 / A;
	if (Delta < 0)
	{
		return std::nullopt;
	}
	float Hit_Result = (-1.f * B - glm::sqrt(Delta)) * 0.5f * InverseA;
	if (Hit_Result < 0)
	{
		Hit_Result = (-1.f * B + glm::sqrt(Delta)) * 0.5f * InverseA;
	}
	if (Hit_Result > T_Min && Hit_Result < T_Max)
	{
		glm::vec3 HitPoint = InRay.Hit(Hit_Result);
		glm::vec3 Normal = glm::normalize(HitPoint - Center);

		HitInfo ResultInfo;
		ResultInfo.TStep = Hit_Result;
		ResultInfo.HitPos = HitPoint;
		ResultInfo.Normal = Normal;

		return ResultInfo;
	}
	return std::nullopt;
}
