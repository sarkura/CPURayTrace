#pragma once

#include "Bounds.h"
#include "Shape.h"

struct ShapeInstance
{
	const Shape* ShapePtr = nullptr;
	const Material* InstanceMaterial = nullptr;
	glm::mat4 WorldFromObject = glm::mat4();
	glm::mat4 ObjectFromWorld = glm::mat4();

	Bounds ShapeBounds{};
	glm::vec3 Center{};

	void UpdateBounds()
	{
		ShapeBounds = {};
		auto BoundsObject = ShapePtr->GetBounds();
		for (size_t Idx = 0; Idx < 8; Idx++)
		{
			auto CornerObject = BoundsObject.GetCorner(Idx);
			glm::vec3 CornerWorld = WorldFromObject * glm::vec4(CornerObject, 1.f);
			ShapeBounds.Expand(CornerWorld);
		}
		Center = (ShapeBounds.BoundsMax + ShapeBounds.BoundsMin) * 0.5f;
	}
};

struct SceneBVHTreeNode
{
public:
	void UpdateBounds();
public:
	Bounds TreeBounds = {};
	std::vector<ShapeInstance> BoundsShapeInstances;
	SceneBVHTreeNode* LeftChildNode = nullptr;
	SceneBVHTreeNode* RightChildNode = nullptr;
	int Depth = 0;
	int SplitAxis = 0;
};

struct alignas(32) SceneBVHNode
{
	Bounds TreeBounds{};
	union
	{
		int RightchildIndex;
		int ShapeInstanceIndex;
	};
	uint16_t ShapeInstanceCount;
	uint8_t SplitAxis;
};

struct SceneBVHState
{
	size_t TotalShapeInstanceCount = 0;
	size_t TotalNodeCount = 0;
	size_t LeafNodeCount = 0;
	size_t LeafNodeMaxShapeInstanceCount = 0;
	size_t MaxLeafNodeDepth = 0;

	void AddLeafNode(SceneBVHTreeNode* InSceneBVHTreeNode)
	{
		LeafNodeCount += 1;
		LeafNodeMaxShapeInstanceCount = glm::max(LeafNodeMaxShapeInstanceCount, InSceneBVHTreeNode->BoundsShapeInstances.size());
		MaxLeafNodeDepth = glm::max(MaxLeafNodeDepth, static_cast<size_t>(InSceneBVHTreeNode->Depth));
	}
};

class SceneBVHTreeNodeAllocator
{
public:
	SceneBVHTreeNodeAllocator();
	~SceneBVHTreeNodeAllocator();

	SceneBVHTreeNode* Allocator();
private:
	size_t BlockSize;
	std::vector<SceneBVHTreeNode*> SceneBVHTreeNodeList;
};


class SceneBVHTree : public Shape
{
public:
	SceneBVHTree() = default;
	void BuildTree(std::vector<ShapeInstance>&& BoundsShapeInstances);

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min, float T_Max) const override;
	virtual Bounds GetBounds() const override;
private:
	void RecurseSplit(SceneBVHTreeNode* SplitNode, SceneBVHState& InSceneBVHState);
	int RecursiveFlatten(SceneBVHTreeNode* FlattenNode);
private:
	std::vector<SceneBVHNode> Nodes;
	std::vector<ShapeInstance> OrderedShapeInstances;
	std::vector<ShapeInstance> InfinityInstances;
	SceneBVHTreeNode* RootNode = nullptr;
	SceneBVHTreeNodeAllocator TreeNodeAllocator{};
};
