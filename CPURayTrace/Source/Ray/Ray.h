#pragma once

#include"glm/glm.hpp"
#include "Material.h"

struct Ray
{
	//Origin
	glm::vec3 Origin = { 0.f, 0.f, 0.f };
	//Direction
	glm::vec3 Direction = { 0.f, 0.f, 0.f };

	glm::vec3 Hit(float T) const;
	Ray ObjectSpaceFromWorldSpace(const glm::mat4& ObjectFromWorld) const;
};


struct HitInfo
{
	float TStep = 0.f;
	glm::vec3 HitPos = {0.f, 0.f, 0.f};
	glm::vec3 Normal = { 0.f, 0.f, 0.f };
	const Material* HitMaterial = nullptr;
};


