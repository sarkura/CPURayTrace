#pragma once
#include "Shape.h"

struct ShapeInstance
{
	const Shape* ShapePtr = nullptr;
	const Material* MaterialPtr = nullptr;
	glm::mat4 WorldFromObject = glm::mat4();
	glm::mat4 ObjectFromWorld = glm::mat4();
};

class Scene : public Shape
{
public:
	Scene() = default;

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min = 1e-5, float T_Max = std::numeric_limits<float>::infinity()) const override;

	void AddShape(const Shape* InShapes, 
		const Material* InMaterial,
		const glm::vec3& InPosition = { 0.f,0.f,0.f },
		const glm::vec3& InScale = {1.f, 1.f, 1.f},
		const glm::vec3& InRotate = { 0.f,0.f,0.f });
private:
	std::vector<ShapeInstance> CollectShapeInstances;
};
