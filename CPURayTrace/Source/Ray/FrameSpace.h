#pragma once

#include "glm/glm.hpp"

class FrameSpace
{
public:
	FrameSpace(const glm::vec3& InNormal);

	glm::vec3 LocalFromWorld(const glm::vec3& InWorldDirection) const;
	glm::vec3 WorldFromLocal(const glm::vec3& InLocalDirection) const;

private:
	glm::vec3 XAXis, YAXis, ZAXis;
};
