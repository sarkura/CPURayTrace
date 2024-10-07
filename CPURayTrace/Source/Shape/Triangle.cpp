#pragma once
#include "Triangle.h"


Triangle::Triangle(const glm::vec3 InVertexPos0, const glm::vec3 InVertexPos1, const glm::vec3 InVertexPos2,
	const glm::vec3 InNormal0, const glm::vec3 InNormal1, const glm::vec3 InNormal2) :
	VertexPos0(InVertexPos0), VertexPos1(InVertexPos1), VertexPos2(InVertexPos2), Normal0(InNormal0), Normal1(InNormal1), Normal2(InNormal2)
{

}

Triangle::Triangle(const glm::vec3 InVertexPos0, const glm::vec3 InVertexPos1, const glm::vec3 InVertexPos2):
	VertexPos0(InVertexPos0), VertexPos1(InVertexPos1), VertexPos2(InVertexPos2)
{
	glm::vec3 Edge1 = InVertexPos1 - InVertexPos0;
	glm::vec3 Edge2 = InVertexPos2 - InVertexPos0;
	glm::vec3 CrossN = glm::cross(Edge1, Edge2);
	Normal0 = CrossN;
	Normal1 = CrossN;
	Normal2 = CrossN;
}

// O: Origin D: Direction t:TStep/Time
// A: TrianmglePos0 B: TrianmglePos1 C: TrianmglePos2

// O + tD == (1 - U - V) * A + U * B + V * C
// U + V + (1 - U - V) = 1
// 0 <= U <= 1 && 0 <= V <= 1 && 0 <= (U + V) <= 1 && 0 <= (1 - U - V) <= 1
// Triangle barycentric coordinate representation
// solve function

// S = O - A
// Edge1 = B - A
// Edge2 = C - A
// (-D, Edge1, Edge2) * Inverse(t, U, V) = S
// S1 = Cross(D, Edge2)
// S2 = Cross(S, Edge1)
// Inverse(t, U, V) = (1 / (S1 * Edge1)) * Inverse(S2 * Edge2, S1 * S, S2 * D)

// Did it need Reduce Calculate?
// Move Calculate Step
std::optional<HitInfo> Triangle::Intersect(const Ray& InRay, float T_Min /*= 1e-5*/, float T_Max /*= std::numeric_limits<float>::infinity()*/) const
{
	glm::vec3 S = InRay.Origin - VertexPos0;
	glm::vec3 Edge1 = VertexPos1 - VertexPos0;
	glm::vec3 Edge2 = VertexPos2 - VertexPos0;

	glm::vec3 S1 = glm::cross(InRay.Direction, Edge2);
	glm::vec3 S2 = glm::cross(S, Edge1);
	
	//(1 / (S1 * Edge1))
	float Value = 1.0f / glm::dot(S1, Edge1);

	float U = glm::dot(S1, S) * Value;
	if (U < 0.f || U > 1.f)
	{
		return std::nullopt;
	}

	float V = glm::dot(S2, InRay.Direction) * Value;
	if (V < 0.f || V + U > 1.f)
	{
		return std::nullopt;
	}

	float Hit_TStep = glm::dot(S2, Edge2) * Value;
	if (Hit_TStep > T_Min && Hit_TStep < T_Max)
	{
		glm::vec3 Hit_Pos = InRay.Hit(Hit_TStep);
		glm::vec3 InterpolatedNormal = (1.f - U - V) * Normal0 + U * Normal1 + V * Normal2;

		HitInfo ResultInfo;
		ResultInfo.TStep = Hit_TStep;
		ResultInfo.HitPos = Hit_Pos;
		ResultInfo.Normal = glm::normalize(InterpolatedNormal);

		return ResultInfo;
	}

	return std::nullopt;
}

Bounds Triangle::GetBounds() const
{
	Bounds ShapeBounds;
	ShapeBounds.Expand(VertexPos0);
	ShapeBounds.Expand(VertexPos1);
	ShapeBounds.Expand(VertexPos2);
	return ShapeBounds;
}
