#pragma once
#include "Bounds.h"
#include "Triangle.h"

struct BVHTreeNode
{
public:
	void UpdateBounds();
public:
	Bounds TreeBounds = {};
	std::vector<Triangle> BoundsTraiangles;
	BVHTreeNode* LeftChildNode = nullptr;
	BVHTreeNode* RightChildNode = nullptr;
};

struct BVHNode
{
	Bounds TreeBounds{};
	size_t RightchildIndex;
	size_t TriangleIndex;
	size_t TriangleCount;
};

class BVHTree : public Shape
{
public:
	BVHTree() = default;
	void BuildTree(std::vector<Triangle>&& BoundsTraiangles);

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min, float T_Max) const override;
private:
	void RecurseSplit(BVHTreeNode* SplitNode);
	size_t RecursiveFlatten(BVHTreeNode* FlattenNode);
private:
	std::vector<BVHNode> Nodes;
	std::vector<Triangle> OrderedTraiangles;
};
