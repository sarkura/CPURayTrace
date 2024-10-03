#include "Scene.h"
#include "glm/ext/matrix_transform.hpp"

std::optional<HitInfo> Scene::Intersect(const Ray& InRay, float T_Min /*= 1e-5*/, float T_Max /*= std::numeric_limits<float>::infinity()*/) const
{
	std::optional<HitInfo> ResultInfo;

	float MaxStep = T_Max;
	const ShapeInstance* ResultShapeInstance = nullptr;

	for (const ShapeInstance& Instance : CollectShapeInstances)
	{
		Ray ObjectSpaceRay = InRay.ObjectSpaceFromWorldSpace(Instance.ObjectFromWorld);
		std::optional<HitInfo> CurHitInfo = Instance.ShapePtr->Intersect(ObjectSpaceRay, T_Min, MaxStep);
		if (CurHitInfo.has_value())
		{
			MaxStep = CurHitInfo->TStep;
			ResultInfo = CurHitInfo;
			ResultShapeInstance = &Instance;
		}
	}

	if (ResultShapeInstance)
	{
		glm::vec3 WorldSpaceHitPos = ResultShapeInstance->WorldFromObject * glm::vec4(ResultInfo->HitPos, 1.0f);
		glm::vec3 WorldSpaceNormal = glm::transpose(ResultShapeInstance->ObjectFromWorld) * glm::vec4(ResultInfo->Normal, 0.0f);

		ResultInfo->HitPos = WorldSpaceHitPos;
		ResultInfo->Normal = glm::normalize(WorldSpaceNormal);
		ResultInfo->HitMaterial = ResultShapeInstance->MaterialPtr;
	}

	return ResultInfo;
}

void Scene::AddShape(const Shape* InShapes, const Material* InMaterial, const glm::vec3& InPosition /*= { 0.f,0.f,0.f }*/, const glm::vec3& InScale /*= {1.f, 1.f, 1.f}*/, const glm::vec3& InRotate /*= { 0.f,0.f,0.f }*/)
{
	glm::mat4 ScaleMx = glm::scale(glm::mat4(1.0f), InScale);
	glm::mat4 RotateXMx = glm::rotate(glm::mat4(1.0f), glm::radians(InRotate.x), { 1.0f, 0.0f, 0.0f });
	glm::mat4 RotateYMx = glm::rotate(glm::mat4(1.0f), glm::radians(InRotate.y), { 0.0f, 1.0f, 0.0f });
	glm::mat4 RotateZMx = glm::rotate(glm::mat4(1.0f), glm::radians(InRotate.z), { 0.0f, 0.0f, 1.0f });
	glm::mat4 TranslateMx = glm::translate(glm::mat4(1.0f), InPosition);
	glm::mat4 WorldFromObject = TranslateMx * RotateZMx * RotateYMx * RotateXMx * ScaleMx;

	ShapeInstance NewShapeInstance;
	NewShapeInstance.ShapePtr = InShapes;
	NewShapeInstance.WorldFromObject = WorldFromObject;
	NewShapeInstance.ObjectFromWorld = glm::inverse(WorldFromObject);
	NewShapeInstance.MaterialPtr = InMaterial;

	CollectShapeInstances.push_back(NewShapeInstance);
}

