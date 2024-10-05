#include "BVH.h"
#include <array>

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
	BVHTreeNode* RootNode = new BVHTreeNode();
	if (RootNode)
	{
		RootNode->BoundsTraiangles = std::move(BoundsTraiangles);
		RootNode->UpdateBounds();
		RecurseSplit(RootNode);
		RecursiveFlatten(RootNode);
	}
}

void BVHTree::RecurseSplit(BVHTreeNode* SplitNode)
{
	if (SplitNode)
	{
		if (SplitNode->BoundsTraiangles.size() == 1)
		{
			return;
		}

		glm::vec3 Diagonal = SplitNode->TreeBounds.GetBoundsDiagonal();
		int MaxAxis = Diagonal.x > Diagonal.y ? (Diagonal.x > Diagonal.z ? 0 : 2) : (Diagonal.y > Diagonal.z ? 1 : 2);
		float MidLocation = SplitNode->TreeBounds.BoundsMin[MaxAxis] + Diagonal[MaxAxis] * 0.5f;

		std::vector<Triangle> LeftNodeTriangles, RightNodeTriangles;
		for (const auto& SplitNodeTriangle : SplitNode->BoundsTraiangles)
		{
			
			if ((SplitNodeTriangle.VertexPos0[MaxAxis] + SplitNodeTriangle.VertexPos1[MaxAxis] + SplitNodeTriangle.VertexPos2[MaxAxis]) / 3.0f < MidLocation)
			{
				LeftNodeTriangles.push_back(SplitNodeTriangle);
			}
			else
			{
				RightNodeTriangles.push_back(SplitNodeTriangle);
			}
		}
		if (LeftNodeTriangles.empty() || RightNodeTriangles.empty())
		{
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

		RecurseSplit(LeftChildNode);
		RecurseSplit(RightChildNode);
	}
}

std::optional<HitInfo> BVHTree::Intersect(const Ray& InRay, float T_Min, float T_Max) const
{
	std::optional<HitInfo> ResultInfo;
	std::array<size_t, 32 > Stack;
	auto Ptr = Stack.begin();
	size_t CurrentNodeIndex = 0;
	while (true)
	{
		auto& Node = Nodes[CurrentNodeIndex];
		if (!Node.TreeBounds.HasIntersection(InRay, T_Min, T_Max))
		{
			if (Ptr == Stack.begin())
			{
				break;
			}
			CurrentNodeIndex = *(--Ptr);
			continue;
		}
		if (Node.BoundsTraiangles.empty())
		{
			CurrentNodeIndex++;
			*(Ptr++) = Node.RightchildIndex;
		}
		else
		{
			for (const auto& BoundsTraiangle : Node.BoundsTraiangles)
			{
				auto TraiangleHitInfo = BoundsTraiangle.Intersect(InRay, T_Min, T_Max);
				if (TraiangleHitInfo.has_value())
				{
					T_Max = TraiangleHitInfo->TStep;
					ResultInfo = TraiangleHitInfo;
				}
			}
			if (Ptr == Stack.begin())
			{
				break;
			}
			CurrentNodeIndex = *(--Ptr);
		}
	}
	return ResultInfo;
}

size_t BVHTree::RecursiveFlatten(BVHTreeNode* FlattenNode)
{
	BVHNode NewBVHNode{
		FlattenNode->TreeBounds,
		std::move(FlattenNode->BoundsTraiangles),
		0
	};
	size_t Index = Nodes.size();
	Nodes.push_back(NewBVHNode);
	if (NewBVHNode.BoundsTraiangles.empty())
	{
		RecursiveFlatten(FlattenNode->LeftChildNode);
		Nodes[Index].RightchildIndex = RecursiveFlatten(FlattenNode->RightChildNode);
	}
	return Index;
}
