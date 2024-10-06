#include "BVH.h"
#include "DebugMacro.hpp"

#include <array>
#include <iostream>

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
	if (BoundsTraiangles.size() == 0)
	{
		return;
	}

	BVHTreeNode* RootNode = new BVHTreeNode();
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
		Bounds MinLeftBounds, MinRightBounds;
		
		int MaxAxis = Diagonal.x > Diagonal.y ? (Diagonal.x > Diagonal.z ? 0 : 2) : (Diagonal.y > Diagonal.z ? 1 : 2);
		SplitNode->SplitAxis = MaxAxis;
		std::vector<Triangle> LeftNodeTriangles, RightNodeTriangles;

		//int SplitBlockCount = static_cast<int>(glm::ceil(Diagonal[MaxAxis] / MinBuildSize)) + 1;
		constexpr int SplitBlockCount = 12;
		for (int i = 0; i < SplitBlockCount; i++)
		{
			float MidLocation = SplitNode->TreeBounds.BoundsMin[MaxAxis] + Diagonal[MaxAxis] * (i + 1.0f) / static_cast<float>(SplitBlockCount);
			std::vector<Triangle> TempLeftNodeTriangles, TempRightNodeTriangles;
			for (const auto& SplitNodeTriangle : SplitNode->BoundsTraiangles)
			{
				if ((SplitNodeTriangle.VertexPos0[MaxAxis] + SplitNodeTriangle.VertexPos1[MaxAxis] + SplitNodeTriangle.VertexPos2[MaxAxis]) / 3.0f < MidLocation)
				{
					MinLeftBounds.Expand(SplitNodeTriangle.VertexPos0);
					MinLeftBounds.Expand(SplitNodeTriangle.VertexPos1);
					MinLeftBounds.Expand(SplitNodeTriangle.VertexPos2);
					TempLeftNodeTriangles.push_back(SplitNodeTriangle);
				}
				else
				{
					MinRightBounds.Expand(SplitNodeTriangle.VertexPos0);
					MinRightBounds.Expand(SplitNodeTriangle.VertexPos1);
					MinRightBounds.Expand(SplitNodeTriangle.VertexPos2);
					TempRightNodeTriangles.push_back(SplitNodeTriangle);
				}
			}
			if (TempLeftNodeTriangles.empty() || TempRightNodeTriangles.empty())
			{
				continue;
			}
			float CurCost = MinLeftBounds.Area() * TempLeftNodeTriangles.size() + MinRightBounds.Area() * TempRightNodeTriangles.size();
			if (CurCost < MinCost)
			{
				MinCost = CurCost;
				LeftNodeTriangles = std::move(TempLeftNodeTriangles);
				RightNodeTriangles = std::move(TempRightNodeTriangles);
			}
		}

		
		if (LeftNodeTriangles.empty() || RightNodeTriangles.empty())
		{
			InBVHState.AddLeafNode(SplitNode);
			return;
		}

		SplitNode->BoundsTraiangles.clear();

		BVHTreeNode* LeftChildNode = new BVHTreeNode();
		BVHTreeNode* RightChildNode = new BVHTreeNode();

		SplitNode->LeftChildNode = LeftChildNode;
		SplitNode->RightChildNode = RightChildNode;

		LeftChildNode->BoundsTraiangles = std::move(LeftNodeTriangles);
		RightChildNode->BoundsTraiangles = std::move(RightNodeTriangles);

		LeftChildNode->UpdateBounds();
		RightChildNode->UpdateBounds();

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
