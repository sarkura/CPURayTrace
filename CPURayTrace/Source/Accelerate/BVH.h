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
	int Depth = 0;
	int SplitAxis = 0;
};

struct alignas(32) BVHNode
{
	Bounds TreeBounds{};
	union
	{
		int RightchildIndex;
		int TriangleIndex;
	};
	uint16_t TriangleCount;
	uint8_t Depth;
	uint8_t SplitAxis;
};

struct BVHState
{
	size_t TotalTriangleCount = 0;
	size_t TotalNodeCount = 0;
	size_t LeafNodeCount = 0;
	size_t LeafNodeMaxTriangleCount = 0;
	size_t MaxLeafNodeDepth = 0;

	void AddLeafNode(BVHTreeNode* InBVHTreeNode)
	{
		LeafNodeCount += 1;
		LeafNodeMaxTriangleCount = glm::max(LeafNodeMaxTriangleCount, InBVHTreeNode->BoundsTraiangles.size());
		MaxLeafNodeDepth = glm::max(MaxLeafNodeDepth, static_cast<size_t>(InBVHTreeNode->Depth));
	}
};

class BVHTreeNodeAllocator
{
public:
	BVHTreeNodeAllocator();
	~BVHTreeNodeAllocator();

	BVHTreeNode* Allocator();
private:
	size_t BlockSize;
	std::vector<BVHTreeNode*> BVHTreeNodeList;
};


class BVHTree : public Shape
{
public:
	BVHTree() = default;
	void BuildTree(std::vector<Triangle>&& BoundsTraiangles);

	virtual std::optional<HitInfo> Intersect(const Ray& InRay, float T_Min, float T_Max) const override;
	virtual Bounds GetBounds() const override;
private:
	void RecurseSplit(BVHTreeNode* SplitNode, BVHState& InBVHState);
	int RecursiveFlatten(BVHTreeNode* FlattenNode);
private:
	std::vector<BVHNode> Nodes;
	std::vector<Triangle> OrderedTraiangles;
	BVHTreeNode* RootNode = nullptr;
	BVHTreeNodeAllocator TreeNodeAllocator{};
};
