#include "FrameSpace.h"

FrameSpace::FrameSpace(const glm::vec3& InNormal)
{
	YAXis = InNormal;

	glm::vec3 CrossVector = abs(YAXis.y) < 0.99999 ? glm::vec3(0, 1, 0) : glm::vec3(0, 0, 1);

	XAXis = glm::cross(CrossVector, YAXis);
	ZAXis = glm::cross(XAXis, YAXis);
}

glm::vec3 FrameSpace::LocalFromWorld(const glm::vec3& InWorldDirection) const
{
	float LocalXDirect = glm::dot(InWorldDirection, XAXis);
	float LocalYDirect = glm::dot(InWorldDirection, YAXis);
	float LocalZDirect = glm::dot(InWorldDirection, ZAXis);

	glm::vec3 LocalDirect = { LocalXDirect, LocalYDirect, LocalZDirect };
	LocalDirect = glm::normalize(LocalDirect);

	return LocalDirect;
}

glm::vec3 FrameSpace::WorldFromLocal(const glm::vec3& InLocalDirection) const
{
	glm::vec3 WorldXDirect = InLocalDirection.x * XAXis;
	glm::vec3 WorldYDirect = InLocalDirection.y * YAXis;
	glm::vec3 WorldZDirect = InLocalDirection.z * ZAXis;

	glm::vec3 WorldDirect = WorldXDirect + WorldYDirect + WorldZDirect;
	WorldDirect = glm::normalize(WorldDirect);

	return WorldDirect;
}
