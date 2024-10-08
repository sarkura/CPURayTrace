#pragma once

#include "glm/glm.hpp"
#include "RandomDistribution.hpp"
#include "ConstantDefine.hpp"

namespace SpheraicalSample
{
	inline glm::vec2 UniformSampleUintDisk(const glm::vec2& UV)
	{
		float R = glm::sqrt(UV.x);
		float Theta = 2.0f * static_cast<float>(PI) * UV.y;
		return { R, Theta };
	}

	inline glm::vec3 CosineSampleHemisphere(const glm::vec2& UV)
	{
		float R = glm::sqrt(UV.x);
		float PHI = 2 * static_cast<float>(PI) * UV.y;
		return { R * glm::cos(PHI), glm::sqrt(1 - R * R), R * glm::sin(PHI) };
	}

	template<typename Tx>
	inline glm::vec3 UniformSampleHemisphere(const RandomDistribution<Tx>& RandHandle)
	{
		glm::vec3 Result{};
		do
		{
			Result = { RandHandle.GetRandom(), RandHandle.GetRandom(), RandHandle.GetRandom() };
			Result = Result * 2.f - 1.0f;
		} while (glm::length(Result) > 1.0f);

		if (Result.y < 0.f)
		{
			Result.y = -Result.y;
		}
		return glm::normalize(Result);
	}
}