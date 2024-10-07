#include "BVH.h"
#include "DebugMacro.hpp"

#include <array>
#include <vector>
#include <iostream>
#include "Profile.h"

void BVHTreeNode::UpdateBounds()
{
	TreeBounds.InvaildBounds();
	for (const auto& CurTriangle : BoundsTraiangles)
	{
		TreeBounds.Expand(CurTriangle.VertexPos0);
		TreeBounds.Expand(CurTriangle.VertexPos1);
		TreeBounds.Expand(CurTriangle.VertexPos2);
	}

}

void BVHTree::BuildTree(std::vector<Triangle>&& BoundsTraiangles)
{
	PROFILE("BuildTree")
	if (BoundsTraiangles.size() == 0)
	{
		return;
	}

	RootNode = TreeNodeAllocator.Allocator();
	if (RootNode)
	{
		RootNode->BoundsTraiangles = std::move(BoundsTraiangles);
		RootNode->UpdateBounds();
		RootNode->Depth = 1;

		BVHState NewState;
		NewState.TotalTriangleCount = RootNode->BoundsTraiangles.size();

		RecurseSplit(RootNode, NewState);

		std::cout << "Total Node Count: " << NewState.TotalNodeCount << std::endl;
		std::cout << "Leaf Node Count: " << NewState.LeafNodeCount << std::endl;
		std::cout << "Triangle Count: " << NewState.TotalTriangleCount << std::endl;
		std::cout << "Mean Leaf Node Triangle Count: " << static_cast<float>(NewState.TotalTriangleCount) / static_cast<float>(NewState.LeafNodeCount) << std::endl;
		std::cout << "Max Leaf Node Triangle Count: " << NewState.LeafNodeMaxTriangleCount << std::endl;

		Nodes.reserve(NewState.TotalNodeCount);
		OrderedTraiangles.reserve(NewState.TotalTriangleCount);
		RecursiveFlatten(RootNode);
	}
}

void BVHTree::RecurseSplit(BVHTreeNode* SplitNode, BVHState& InBVHState)
{
	if (SplitNode)
	{
		InBVHState.TotalNodeCount += 1;
		if (SplitNode->BoundsTraiangles.size() == 1 || SplitNode->Depth > 32)
		{
			InBVHState.AddLeafNode(SplitNode);
			return;
		}	

		glm::vec3 Diagonal = SplitNode->TreeBounds.GetBoundsDiagonal();
		float MinCost = std::numeric_limits<float>::infinity();
		size_t MinSplitIndex = 0;
		Bounds LeftChildBound, RightChildBound;
		size_t LeftTriangleCount = 0, RightTriangleCount = 0;
		constexpr int SplitBucketCount = 12;
		std::vector<size_t> BucketTriangleIndex[3][SplitBucketCount] = {};
		
		for (int Axis = 0; Axis < 3; Axis++)
		{
			Bounds BucketBounds[SplitBucketCount] = {};
			size_t BucketTriangleCount[SplitBucketCount] = {0};
			size_t TriangleIndex = 0;
			for (const Triangle& BoundsTraiangle : SplitNode->BoundsTraiangles)
			{
				float BarycentricCoordinates = (BoundsTraiangle.VertexPos0[Axis] + BoundsTraiangle.VertexPos1[Axis] + BoundsTraiangle.VertexPos2[Axis]) / 3.0f;
				size_t BucketId = glm::clamp<size_t>(
					static_cast<size_t>(glm::floor((BarycentricCoordinates - SplitNode->TreeBounds.BoundsMin[Axis]) * SplitBucketCount / Diagonal[Axis])),
					0, SplitBucketCount - 1);
				BucketBounds[BucketId].Expand(BoundsTraiangle.VertexPos0);
				BucketBounds[BucketId].Expand(BoundsTraiangle.VertexPos1);
				BucketBounds[BucketId].Expand(BoundsTraiangle.VertexPos2);
				BucketTriangleCount[BucketId] += 1;
				BucketTriangleIndex[Axis][BucketId].push_back(TriangleIndex);
				TriangleIndex += 1;
			}

			Bounds LeftBounds = BucketBounds[0];
			size_t LeftBoundsTriangleCount = BucketTriangleCount[0];
			for (size_t i = 1; i <= SplitBucketCount - 1; i++)
			{
				Bounds RightBounds;
				size_t RightBoundsTriangleCount = 0;
				for (size_t j = SplitBucketCount - 1; j >=i; j--)
				{
					RightBounds.Expand(BucketBounds[j]);
					RightBoundsTriangleCount += BucketTriangleCount[j];
				}
				if (RightBoundsTriangleCount == 0)
				{
					break;
				}
				if (LeftBoundsTriangleCount != 0)
				{
					float Cost = LeftBounds.Area() * LeftBoundsTriangleCount + RightBounds.Area() * RightBoundsTriangleCount;
					if (Cost < MinCost)
					{
						MinCost = Cost;
						SplitNode->SplitAxis = Axis;
						MinSplitIndex = i;
						LeftChildBound = LeftBounds;
						RightChildBound = RightBounds;
						LeftTriangleCount = LeftBoundsTriangleCount;
						RightTriangleCount = RightBoundsTriangleCount;
					}
				}
				LeftBounds.Expand(BucketBounds[i]);
				LeftBoundsTriangleCount += BucketTriangleCount[i];
			}
		}
		
		if (MinSplitIndex == 0)
		{
			InBVHState.AddLeafNode(SplitNode);
			return;
		}

		//SplitNode->BoundsTraiangles.clear();

		BVHTreeNode* LeftChildNode = TreeNodeAllocator.Allocator();
		BVHTreeNode* RightChildNode = TreeNodeAllocator.Allocator();

		SplitNode->LeftChildNode = LeftChildNode;
		SplitNode->RightChildNode = RightChildNode;

		LeftChildNode->BoundsTraiangles.reserve(LeftTriangleCount);
		RightChildNode->BoundsTraiangles.reserve(RightTriangleCount);

		for (size_t i = 0; i < MinSplitIndex; i++)
		{
			for (size_t Index : BucketTriangleIndex[SplitNode->SplitAxis][i])
			{
				LeftChildNode->BoundsTraiangles.push_back(SplitNode->BoundsTraiangles[Index]);
			}
		}
		for (size_t i = MinSplitIndex; i < SplitBucketCount; i++)
		{
			for (size_t Index : BucketTriangleIndex[SplitNode->SplitAxis][i])
			{
				RightChildNode->BoundsTraiangles.push_back(SplitNode->BoundsTraiangles[Index]);
			}
		}

		SplitNode->BoundsTraiangles.clear();
		SplitNode->BoundsTraiangles.shrink_to_fit();

		LeftChildNode->TreeBounds = LeftChildBound;
		RightChildNode->TreeBounds = RightChildBound;

		LeftChildNode->Depth = SplitNode->Depth + 1;
		RightChildNode->Depth = SplitNode->Depth + 1;

		RecurseSplit(LeftChildNode, InBVHState);
		RecurseSplit(RightChildNode, InBVHState);
	}
}

std::optional<HitInfo> BVHTree::Intersect(const Ray& InRay, float T_Min, float T_Max) const
{
	
	DEBUG_LINE(size_t BoundsTestCount = 0);
	DEBUG_LINE(size_t TriangleTestCount = 0);

	glm::bvec3 RayNearDircetion = { InRay.Direction.x < 0, InRay.Direction.y < 0, InRay.Direction.z < 0 };

	glm::vec3 InverseRayDirection = 1.0f / InRay.Direction;

	std::optional<HitInfo> ResultInfo;
	std::array<size_t, 32 > Stack;
	auto Ptr = Stack.begin();
	size_t CurrentNodeIndex = 0;
	while (true)
	{
		auto& Node = Nodes[CurrentNodeIndex];

		DEBUG_LINE(BoundsTestCount++);

		if (!Node.TreeBounds.HasIntersection(InRay, InverseRayDirection, T_Min, T_Max))
		{
			if (Ptr == Stack.begin())
			{
				break;
			}
			CurrentNodeIndex = *(--Ptr);
			continue;
		}
		if (Node.TriangleCount == 0)
		{
			if (RayNearDircetion[Node.SplitAxis])
			{
				*(Ptr++) = CurrentNodeIndex + 1;
				CurrentNodeIndex = Node.RightchildIndex;
			}
			else
			{
				CurrentNodeIndex++;
				*(Ptr++) = Node.RightchildIndex;
			}
		}
		else
		{
			auto TriangleIterator = OrderedTraiangles.begin() + Node.TriangleIndex;

			DEBUG_LINE(TriangleTestCount += Node.TriangleCount)

			for (size_t i = 0; i < Node.TriangleCount; i++)
			{
				auto TraiangleHitInfo = TriangleIterator->Intersect(InRay, T_Min, T_Max);
				++TriangleIterator;
				if (TraiangleHitInfo.has_value())
				{
					T_Max = TraiangleHitInfo->TStep;
					ResultInfo = TraiangleHitInfo;

					DEBUG_LINE(ResultInfo->BoundsDepth = Node.Depth);

				}
			}
			if (Ptr == Stack.begin())
			{
				break;
			}
			CurrentNodeIndex = *(--Ptr);
		}
	}
	if (ResultInfo.has_value())
	{
		DEBUG_LINE(ResultInfo->BoundsTestCount = BoundsTestCount);
		DEBUG_LINE(ResultInfo->TriangleTestCount = TriangleTestCount);
	}
	return ResultInfo;
}

int BVHTree::RecursiveFlatten(BVHTreeNode* FlattenNode)
{
	BVHNode NewBVHNode{ FlattenNode->TreeBounds, 0, static_cast<uint16_t>(FlattenNode->BoundsTraiangles.size()), 
		static_cast<uint8_t>(FlattenNode->Depth), static_cast<uint8_t>(FlattenNode->SplitAxis)};
	int Index = static_cast<int>(Nodes.size());
	Nodes.push_back(NewBVHNode);
	if (NewBVHNode.TriangleCount == 0)
	{
		RecursiveFlatten(FlattenNode->LeftChildNode);
		Nodes[Index].RightchildIndex = RecursiveFlatten(FlattenNode->RightChildNode);
	}
	else
	{
		Nodes[Index].TriangleIndex = static_cast<int>(OrderedTraiangles.size());
		for (const Triangle& NodeTriangle : FlattenNode->BoundsTraiangles)
		{
			OrderedTraiangles.push_back(NodeTriangle);
		}
	}
	return Index;
}

BVHTreeNodeAllocator::BVHTreeNodeAllocator()
	:BlockSize(4096)
{

}

BVHTreeNodeAllocator::~BVHTreeNodeAllocator()
{
	for (auto* Nodes : BVHTreeNodeList)
	{
		delete[] Nodes;
	}
	BVHTreeNodeList.clear();
	BVHTreeNodeList.shrink_to_fit();
}

BVHTreeNode* BVHTreeNodeAllocator::Allocator()
{
	if (BlockSize == 4096)
	{
		BVHTreeNodeList.push_back(new BVHTreeNode[4096]);
		BlockSize = 0;
	}
	return &(BVHTreeNodeList.back())[BlockSize++];
}
