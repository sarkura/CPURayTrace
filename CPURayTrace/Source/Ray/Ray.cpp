#include "Ray.h"

glm::vec3 Ray::Hit(float T) const
{
	return Origin + T * Direction;
}

Ray Ray::ObjectSpaceFromWorldSpace(const glm::mat4& ObjectFromWorld) const
{
	glm::vec3 ObjectSpaceOrigin = ObjectFromWorld * glm::vec4(Origin, 1.0f);
	glm::vec3 ObjectSpaceDirection = ObjectFromWorld * glm::vec4(Direction, 0.0f);
	Ray ObjectSpaceRay;
	ObjectSpaceRay.Origin = ObjectSpaceOrigin;
	ObjectSpaceRay.Direction = ObjectSpaceDirection;
	return ObjectSpaceRay;
}
